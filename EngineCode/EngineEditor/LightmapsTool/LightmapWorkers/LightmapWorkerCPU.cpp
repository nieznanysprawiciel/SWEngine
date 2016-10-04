#include "EngineEditor/LightmapsTool/stdafx.h"
#include "LightmapWorkerCPU.h"
#include "EngineCore/ModelsManager/DefaultAssets.h"

#include <cstring>
#include <limits>
// Trzeba oddefiniowaæ, je¿eli chce siê korzystaæ z numeric_limits
#undef max

#include "Common/MemoryLeaks.h"

using namespace DirectX;

LightmapWorkerCPU::LightmapWorkerCPU( SceneData* sceneData )
	: LightmapWorker( sceneData )
{
	m_threshold = 0.04f;
}

// ============================================================================= //
// Helpers
// ============================================================================= //

/**@brief Szybsze (miejmy nadziejê) mno¿enie przez 3.*/
inline unsigned int mul3( unsigned int number )
{	return ( number << 1 ) + number;	}

/**@brief Dodaje pod podanymi indeksami w chunku element addValue.*/
inline XMFLOAT3& LoadAddStore( std::vector<MemoryChunk>& chunk, unsigned int i, unsigned int j, XMVECTOR addValue )
{
	auto& lightValueRef = chunk[ i ].Get<XMFLOAT3>( j );
	XMVECTOR lightValue = XMLoadFloat3( &lightValueRef );
	lightValue = XMVectorAdd( addValue, lightValue );
	XMStoreFloat3( &lightValueRef, lightValue );
	
	return lightValueRef;
}


inline void CompareEmission( std::tuple<unsigned int, unsigned int, float>& prevEmission, unsigned int i, unsigned int j, const XMFLOAT3& newEmission )
{
	if( newEmission.x > std::get<2>( prevEmission ) )
		prevEmission = std::make_tuple( i, j, newEmission.x );
	if( newEmission.y > std::get<2>( prevEmission ) )
		prevEmission = std::make_tuple( i, j, newEmission.y );
	if( newEmission.z > std::get<2>( prevEmission ) )
		prevEmission = std::make_tuple( i, j, newEmission.z );
}

std::tuple<unsigned int, unsigned int, float> FindMaxEmission( std::vector<MemoryChunk>& emissionLight )
{
	std::tuple<unsigned int, unsigned int, float> emissionMax = std::make_tuple( 0, 0, -1.0f );
	for( unsigned int i = 0 ; i < emissionLight.size(); ++i )
		for( unsigned int j = 0; j < emissionLight[ i ].Count<XMFLOAT3>(); ++j )
		{
			auto& emissionVector = emissionLight[ i ].Get<XMFLOAT3>( j );
			CompareEmission( emissionMax, i, j, emissionVector );
		}
	return emissionMax;
}

/**@brief Czyœci bufory g³êbokoœci i indeksów.*/
void ClearDepthIndex( float* depth, BufferIndexing* index, unsigned int elements )
{
	for( unsigned int i = 0; i < elements; ++i )
		depth[ i ] = std::numeric_limits<float>::max();
	for( unsigned int i = 0; i < elements; ++i )
	{
		index[ i ].first = INVALID_INDEX;
		index[ i ].second = INVALID_INDEX;
	}
}

/**@brief Maksymalna wartoœæ trzech liczb.*/
int max3( float x1, float x2, float x3 )
{
	if( x1 > x2 )
		return x1 > x3 ? (int)x1 : (int)x3;
	else
		return x2 > x3 ? (int)x2 : (int)x3;
}

/**@brief Minimalna wartoœæ trzech liczb.*/
int min3( float x1, float x2, float x3 )
{
	if( x1 < x2 )
		return x1 < x3 ? (int)x1 : (int)x3;
	else
		return x2 < x3 ? (int)x2 : (int)x3;
}


// ============================================================================= //
// Generation
// ============================================================================= //

/**@brief G³ówna funkcja generuj¹ca lightmapy. Zaimplementuj w klasie pochodnej.*/
void LightmapWorkerCPU::Generate		()
{
	if( m_data->objectParts.empty() )
		return;

	// Chunki dla ca³ych obieków (nie dla podczêœci)
	std::vector<MemoryChunk> emissionLight;
	std::vector<MemoryChunk> reachedLight;
	std::vector<MemoryChunk> verticies;			// Przekszta³cone wierzcho³ki i normalne.

	Prepare( emissionLight, reachedLight, verticies );
	Radiosity( emissionLight, reachedLight, verticies );
	BuildResult( reachedLight );

	m_lightmapState = LightmapState::GenerationEnded;		// Ustaw na koñcu funkcji, ¿eby w¹tek g³ówny móg³ zebraæ wyniki.
}

/**@brief Przygotowuje odpowiednie struktury do generacji.*/
void LightmapWorkerCPU::Prepare( std::vector<MemoryChunk>& emissionLight, std::vector<MemoryChunk>& reachedLight, std::vector<MemoryChunk>& verticies )
{
	auto& parts = m_data->objectParts;
	for( auto& part : parts )
	{
		unsigned int chunkSize = part.verticesCount * sizeof( XMFLOAT3 ) / 3;

		MemoryChunk emissionChunk( chunkSize );
		MemoryChunk reachedChunk( chunkSize );
		MemoryChunk verticiesChunk( part.verticesCount * sizeof( VertexFormat ) );

		memset( reachedChunk.GetMemory<void>(), 0, chunkSize );
		memset( emissionChunk.GetMemory<void>(), 0, chunkSize );		// Potem wype³nimy wartoœciami emisji w pêtli.

		XMMATRIX transformMatrix = XMLoadFloat4x4( &part.transform );

		// Transform positions
		XMVector3TransformCoordStream(	&verticiesChunk.Get<VertexFormat>( 0 ).position,
										sizeof( VertexFormat ),
										&m_data->verticies[ part.chunkIdx ].Get<VertexNormalTexCoord>( part.bufferOffset ).Position,
										sizeof( VertexNormalTexCoord ),
										part.verticesCount,
										transformMatrix );

		// Transform normals
		XMVector3TransformNormalStream( &verticiesChunk.Get<VertexFormat>( 0 ).normal,
										sizeof( VertexFormat ),
										&m_data->verticies[ part.chunkIdx ].Get<VertexNormalTexCoord>( part.bufferOffset ).Normal,
										sizeof( VertexNormalTexCoord ),
										part.verticesCount,
										transformMatrix );

		if( part.emissive.x != 0.0f || part.emissive.y != 0.0f || part.emissive.z != 0.0f )
		{
			XMFLOAT3 materialEmissive( part.emissive.x, part.emissive.y, part.emissive.z );

			unsigned int trianglesCount = part.verticesCount / 3;

			// To takie skalowanie na oko troszkê. W ka¿dym razie zak³adamy, ¿e œwiat³o o mocy (1,1,1) powinno wypromieniowaæ
			// tyle samo energii niezale¿nie od iloœci wierzcho³ków. W przysz³oœci trzeba bêdzie jakoœ pewnie uwzglêdniaæ powierzchniê
			// trójk¹tów zarówno emituj¹cych jak i odbieracj¹cych. Tych obliczeñ widzê robi siê coraz wiêcej.
			//float scaleFactor = 1.0f/*(float)m_depthResolution *// (float)trianglesCount;
			//materialEmissive.x = materialEmissive.x * scaleFactor;
			//materialEmissive.y = materialEmissive.y * scaleFactor;
			//materialEmissive.z = materialEmissive.z * scaleFactor;
			
			for( unsigned int k = 0; k < trianglesCount; ++k )
				emissionChunk.Get<XMFLOAT3>( k ) = materialEmissive;
		}

		emissionLight.push_back( std::move( emissionChunk ) );
		reachedLight.push_back( std::move( reachedChunk ) );
		verticies.push_back( std::move( verticiesChunk ) );
	}
}


/**@brief Wykonuje algorytm radiosity.*/
void LightmapWorkerCPU::Radiosity( std::vector<MemoryChunk>& emissionLight,
								   std::vector<MemoryChunk>& reachedLight,
								   std::vector<MemoryChunk>& verticies )
{
	// Identyfikuje wielok¹t z najwiêksz¹ energi¹ do wyemitowania.
	std::tuple<unsigned int, unsigned int, float> emissionMax = FindMaxEmission( emissionLight );

	//std::get<1>( emissionMax ) = 13;		// Testttttt

	// Bufor g³êbokoœci i bufor s³u¿¹cy do indeksowania trójk¹tów, które znajduj¹ siê w buforze.
	unsigned int		depthSize	= m_depthResolution * m_depthResolution;
	MemoryChunk			depthBuffer( depthSize * sizeof( float) );
	MemoryChunk			indexBuffer( depthSize * sizeof( BufferIndexing ) );

	// Koñczymy generowanie, gdy najwiêksza zgromadzona w wielok¹cie energia spadnie poni¿ej pewnego poziomu.
	int iterations = 0;	
	/*while( std::get<2>( emissionMax ) > m_threshold )*/
	/*while( std::get<2>( emissionMax ) < 1 )*/
	while( iterations < 30 )
	{
		iterations++;

		// Czyœcimy bufory
		ClearDepthIndex( depthBuffer.GetMemory<float>(), indexBuffer.GetMemory<BufferIndexing>(), depthSize );

		DepthPass( emissionMax, verticies, depthBuffer, indexBuffer );
		TransferPass( emissionMax, verticies, emissionLight, reachedLight, indexBuffer );

		emissionMax = FindMaxEmission( emissionLight );
	}
}

/**@brief Tworzy tablicê color - uv, która zostanie potem u¿yta do wyrenderowania lightmapy.

Wszystkie chunki podzielone wczeœniej na podczêœci s¹ teraz ³¹czone. Ka¿dy obiekt bêdzie mia³ swój bufor.*/
void LightmapWorkerCPU::BuildResult	( std::vector<MemoryChunk>& reachedLight )
{
	auto& parts = m_data->objectParts;
	auto object = parts[0].object;
	unsigned int verticiesCount = 0;
	unsigned int firstPartIndex = 0;

	for( unsigned int i = 0; i < parts.size(); ++i )
	{
		verticiesCount += parts[i].verticesCount;

		// Sumujemy liczbê wierzcho³ków w obiekcie. Dopiero jak zacznie siê nowy obiekt to tworzymy bufor.
		unsigned int nextIdx = i+1;
		if( nextIdx == parts.size() || parts[nextIdx].object != object )	// Pierwszy warunek wymusza, ¿e weŸmiemy tak¿e ostatni obiekt
		{
			if( verticiesCount != 0 )
			{
				MemoryChunk colorMap( verticiesCount * sizeof( CoordColor ) );

				unsigned int verticiesOffset = 0;
				for( unsigned int j = firstPartIndex; j < nextIdx; ++j )
				{
					for( unsigned int k = 0; k < reachedLight[ j ].Count<XMFLOAT3>(); ++k )
					{
						CoordColor& colorVertex1 = colorMap.Get<CoordColor>( verticiesOffset + mul3( k ) );		// Jest 3 razy wiêcej wierzcho³ków ni¿ kolorów.
						XMFLOAT3& lightColor = reachedLight[ j ].Get<XMFLOAT3>( k ); /*XMFLOAT3( 0.5, 0.3, 0.7);*/
						colorVertex1.Color = lightColor;
						colorVertex1.TexCoord = m_data->verticies[ parts[j].chunkIdx ].Get<VertexNormalTexCoord>( parts[j].bufferOffset + mul3( k ) ).TexCoord;

						CoordColor& colorVertex2 = colorMap.Get<CoordColor>( verticiesOffset + mul3( k ) + 1 );
						colorVertex2.Color = lightColor;
						colorVertex2.TexCoord = m_data->verticies[ parts[j].chunkIdx ].Get<VertexNormalTexCoord>( parts[j].bufferOffset + mul3( k ) + 1 ).TexCoord;

						CoordColor& colorVertex3 = colorMap.Get<CoordColor>( verticiesOffset + mul3( k ) + 2 );
						colorVertex3.Color = lightColor;
						colorVertex3.TexCoord = m_data->verticies[ parts[j].chunkIdx ].Get<VertexNormalTexCoord>( parts[j].bufferOffset + mul3( k ) + 2 ).TexCoord;
					}

					verticiesOffset += parts[j].verticesCount;
				}

				m_resultData.push_back( std::move( colorMap ) );
			}

			if( nextIdx == parts.size() )		break;		// Nie ma wiêcej obiektów.
			// Zakoñczyliœmy przetwarzanie obiektu przechodzimy do nastêpnego.
			object = parts[nextIdx].object;
			firstPartIndex = nextIdx;
			verticiesCount = 0;
		}
			
	}
}

/**@brief Wykonuje przebieg z z-buforem.*/
void LightmapWorkerCPU::DepthPass( std::tuple<unsigned int, unsigned int, float>& emissionMax,
								   std::vector<MemoryChunk>& verticies,
								   MemoryChunk& depthBuffer,
								   MemoryChunk& indexBuffer )
{
	// Pobieramy indeksy emitera.
	unsigned int idx1 = std::get<0>( emissionMax );
	unsigned int idx2 = std::get<1>( emissionMax );

	uint32 depthFailCounter = 0;
	uint32 triangleCounter = 0;

	// Liczmymy pozycjê emitera, uœrednion¹ normaln¹ i uk³ad wspó³rzêdnych zwi¹zany z emiterem dla z-bufora.
	Triangle4 emiterPosition( &verticies[ idx1 ].Get<VertexFormat>( mul3( idx2 ) ) );
	//Triangle4 emiterCoordSystem = EmiterCoordinatesSystem( emiterPosition );
	//XMVECTOR emiterNormal = AverageNormal( &verticies[ idx1 ].Get<VertexFormat>( mul3( idx2 ) ) );	// Mo¿liwe, ¿e niepotrzebne. emiterCoordSystem ma normaln¹, ale wyliczon¹ inaczej.
	XMMATRIX emiterViewMatrix = EmiterViewMatrix( emiterPosition );

	for( unsigned int i = 0 ; i < verticies.size(); ++i )
	{
		for( unsigned int j = 0; j < verticies[ i ].Count<VertexFormat>() / 3; ++j )
		{
			if( idx2 == j && idx1 == i )
				continue;	// Nie próbujmy rzutowaæ emitera na samego siebie.

			triangleCounter++;

			// Wyliczamy pozycjê odbiorcy w uk³adzie wspó³rzêdnych bufora g³êbokoœci.
			Triangle4 receiverPosition( &verticies[ i ].Get<VertexFormat>( mul3( j ) ) );
			XMFLOAT3 receiverDepths = HemisphereCast( emiterPosition, receiverPosition, emiterViewMatrix );
			if( receiverDepths.x < 0.0f || receiverDepths.y < 0.0f || receiverDepths.z < 0.0f )
			{
				depthFailCounter++;
				continue;		// Trójk¹t jest czêœciowo za p³aszczyzn¹. Musimy go odrzuciæ.
			}

			HemisphereViewport( receiverPosition );
			RasterizeTriangle( receiverPosition, &receiverDepths, i, j, depthBuffer, indexBuffer );
		}

	}
}

void LightmapWorkerCPU::TransferPass( std::tuple<unsigned int, unsigned int, float>& emissionMax,
									  std::vector<MemoryChunk>& verticies,
									  std::vector<MemoryChunk>& emissionLight,
									  std::vector<MemoryChunk>& reachedLight,
									  MemoryChunk& indexBuffer )
{
	// Pobieramy indeks emitera i zerujemy maksymaln¹ emisjê. Bêdziemy szukaæ nowej
	// maksymalnej wartoœci w tym przebiegu.
	std::get<2>( emissionMax ) = 0.0f;
	unsigned int idx1 = std::get<0>( emissionMax );
	unsigned int idx2 = std::get<1>( emissionMax );

	unsigned int depthSize = m_depthResolution * m_depthResolution;

	// Wyliczamy ile energii powinien dostaæ ka¿dy element, który znajdziemy w buforze indeksów.
	XMVECTOR depthElementWeight = XMVectorReplicate( static_cast<float>( 50.0 * 4.0 * XM_1DIV2PI / depthSize ) );
	XMVECTOR emitedLight = XMLoadFloat3( &emissionLight[ idx1 ].Get<XMFLOAT3>( idx2 ) );
	emitedLight = XMVectorMultiply( emitedLight, depthElementWeight );

	// Zerujemy œwiat³o, które emiter bêdzie móg³ wyemitowaæ w kolejnych iteracjach.
	XMStoreFloat3( &emissionLight[ idx1 ].Get<XMFLOAT3>( idx2 ), XMVectorZero() );

	for( unsigned int i = 0; i < depthSize; ++i )
	{
		BufferIndexing indicies = indexBuffer.Get<BufferIndexing>( i );
		// Tylko piksele na kole powinny dostaæ dawkê energii, a bufor jest kwadratem.
		// Dlatego elementy, na które nie zosta³ zrzutowany ¿aden trójk¹t zosta³y zainicjowane wartoœci¹ INVALID_INDEX.
		if( indicies.first != INVALID_INDEX )
		{
			XMVECTOR materialDiffuse = XMLoadFloat4( &m_data->objectParts[ indicies.first ].diffuse );
			XMVECTOR receivedLight = XMVectorMultiply( materialDiffuse, emitedLight );
			
			//receivedLight = XMVectorSet( 1.0f, 0.0f, 0.0f, 0.0f );		// shitty test

			const XMFLOAT3& emissionPower = LoadAddStore( emissionLight, indicies.first, indicies.second, receivedLight );
			LoadAddStore( reachedLight, indicies.first, indicies.second, receivedLight );
			//CompareEmission( emissionMax, indicies.first, indicies.second, emissionPower );
		}
	}
	//// another shitty test
	//XMVECTOR receivedLight = XMVectorSet( 1.0f, 1.0f, 1.0f, 1.0f );
	//LoadAddStore( reachedLight, idx1, idx2, receivedLight );
}


/**@brief Wylicza wspó³rzêdne trójk¹ta zrzutowanego na p³aszczyznê ko³a.
Wspó³rzêdne s¹ dwuwymiarowe, co pozwala ³atwo przejœæ z nich na indeksy do bufora
g³êbokoœci.

@param[inout] receiver Trójk¹t do zrzutowania. W tej zmiennej zwracany jest trójk¹t wynikowy.
@return Zwraca wektor z g³êbokoœciami dla ka¿dego trójk¹ta. Je¿eli jakiœ wierzcho³ek
znajduje siê za p³aszczyzn¹ rzutowania, funkcja zwraca (-1.0, -1.0, -1.0). W normalnej sytuacji
wszystkie wspó³rzêdne s¹ dodatnie.*/
DirectX::XMFLOAT3 LightmapWorkerCPU::HemisphereCast( Triangle4& emiter, Triangle4& receiver, XMMATRIX& emiterViewMatrix )
{
	// Poniewa¿ to s¹ wierzcho³ki, to ustawiamy komponent w na 1.
	receiver.vertex1 = XMVectorSetW( receiver.vertex1, 1.0f );
	receiver.vertex2 = XMVectorSetW( receiver.vertex2, 1.0f );
	receiver.vertex3 = XMVectorSetW( receiver.vertex3, 1.0f );

	// Transformujemy wierzcho³ki do uk³adu emitera. Dziêki temu xy jest pozycj¹ na p³aszczyŸnie (na razie jeszcze nie jednostkowej)
	// a z jest odleg³oœci¹ od emitera.
	XMVECTOR centerToVertex1 = XMVector4Transform( receiver.vertex1, emiterViewMatrix );
	XMVECTOR centerToVertex2 = XMVector4Transform( receiver.vertex2, emiterViewMatrix );
	XMVECTOR centerToVertex3 = XMVector4Transform( receiver.vertex3, emiterViewMatrix );

	// Wyliczenie d³ugoœci od œrodka do wierzcho³ków.
	XMVECTOR depth1 = XMVector3Length( centerToVertex1 );
	XMVECTOR depth2 = XMVector3Length( centerToVertex2 );
	XMVECTOR depth3 = XMVector3Length( centerToVertex3 );

	// Normalizacja wektorów
	receiver.vertex1 = XMVectorDivide( centerToVertex1, depth1 );
	receiver.vertex2 = XMVectorDivide( centerToVertex2, depth2 );
	receiver.vertex3 = XMVectorDivide( centerToVertex3, depth3 );

	XMFLOAT3 depthVector;
	
	depthVector.x = -XMVectorGetZ( centerToVertex1 );
	depthVector.y = -XMVectorGetZ( centerToVertex2 );
	depthVector.z = -XMVectorGetZ( centerToVertex3 );

	//if( XMVectorGetZ( centerToVertex1 ) > 0 )
	//	depthVector.x = -XMVectorGetX( depth1 );
	//else
	//	depthVector.x = XMVectorGetX( depth1 );

	//if( XMVectorGetZ( centerToVertex2 ) > 0 )
	//	depthVector.y = -XMVectorGetX( depth2 );
	//else
	//	depthVector.y = XMVectorGetX( depth2 );
	//
	//if( XMVectorGetZ( centerToVertex3 ) > 0 )
	//	depthVector.z = -XMVectorGetX( depth3 );
	//else
	//	depthVector.z = XMVectorGetX( depth3 );

	return depthVector;
}

/**@brief Wyliczamy wektory równoleg³e do p³aszczyzny i wzajemnie ortogonalne.

Pos³u¿¹ one za uk³ad wspó³rzêdnych, w którym bêdzie wyra¿ony rzut trójk¹ta.
W zasadzie ich orientacja jest dowolna, wa¿ne ¿eby by³y stosowane te same
wektory do wszystkich rzutów.*/
Triangle4 LightmapWorkerCPU::EmiterCoordinatesSystem( Triangle4& emiter )
{
	XMVECTOR edge12 = XMVectorSubtract( emiter.vertex2, emiter.vertex1 );
	XMVECTOR edge13 = XMVectorSubtract( emiter.vertex3, emiter.vertex1 );

	// Wyliczamy najpierw wektor normalny do p³aszczyzny, z potem tworzymy 2 kolejne wektory ortogonalne.
	Triangle4 CoordSystem;
	CoordSystem.vertex1 = XMVector3Normalize( XMVector3Cross( edge13, edge12 ) );
	CoordSystem.vertex2 = XMVector3Normalize( edge12 );															// Jedn¹ z osi wybieramy totalnie dowolnie, wa¿ne by by³a na p³aszczyŸnie.
	CoordSystem.vertex3 = XMVector3Normalize( XMVector3Cross( CoordSystem.vertex2, CoordSystem.vertex1 ) );		// Obliczamy wektor prostopad³y do normalnej i pierwszej osi.

	return CoordSystem;
}

/**@brief Tworzy macierz przekszta³caj¹c¹ wierzcho³ki do uk³adu odniesienia emitera.*/
DirectX::XMMATRIX LightmapWorkerCPU::EmiterViewMatrix( Triangle4& emiter )
{
	// Œrodek ciê¿koœci trójk¹ta przyjujemy za œrodek emitera.
	XMVECTOR emiterCenter = XMVectorAdd( emiter.vertex1, emiter.vertex2 );
	emiterCenter = XMVectorAdd( emiterCenter, emiter.vertex3 );
	emiterCenter = XMVectorDivide( emiterCenter, XMVectorReplicate( 3.0f ) );

	// Wektor prostopad³y do p³aszczyzny, na której le¿¹ trójk¹ty.
	XMVECTOR edge12 = XMVectorSubtract( emiter.vertex2, emiter.vertex1 );
	XMVECTOR edge13 = XMVectorSubtract( emiter.vertex3, emiter.vertex1 );
	XMVECTOR normal = XMVector3Normalize( XMVector3Cross( edge12, edge13 ) );

	// Ustalamy dowolnie (byle ortogonalnie) kierunek "w górê".
	//XMVECTOR upDirection = XMVector3Normalize( XMVector3Cross( edge12, normal ) );
	XMVECTOR upDirection = XMVector3Orthogonal( normal );

	//XMVECTOR test = XMVector3Dot( normal, upDirection );

	// Tworzymy macierz widoku.
	normal = XMVectorSetW( normal, 0.0f );
	emiterCenter = XMVectorSetW( emiterCenter, 1.0f );
	upDirection = XMVectorSetW( upDirection, 0.0f );

	XMMATRIX viewMatrix = XMMatrixLookToRH( emiterCenter, normal, upDirection );
	return viewMatrix;
}

/**@brief Rasteryzuje podany trójk¹t. Sprawdza czy jest bli¿ej emitera od ostatnio
wpisanego trójk¹ta na podstawie bufora g³êbokoœci i je¿eli tak, to zapisuje nowy trójk¹t.

Rasteryzacja zrobiona mniej wiêcej na podstawie strony:
https://fgiesen.wordpress.com/2013/02/06/the-barycentric-conspirac/
Warto przeczytaæ te¿ kilka tematów pokrewnych. Na razie implementujê wersjê niewydajn¹.
Mo¿e potem przerobiê na lepsz¹. W ka¿dym razie warto zauwa¿yæ, ¿e ten algorytm jest bardzo dobry
do zrównoleglania, mo¿e da siê to jakoœ sprytnie wykorzystaæ.

@param[in] triangle Trójk¹t do rasteryzacji.
@param[in] depths Odleg³oœci (g³êbokoœci) kolejnych wierzcho³ków trójk¹ta od emitera. (Tu jest ma³e oszustwo,
zobaczymy jak bêdzie siê sprawdzaæ, odleg³oœci s¹ liczone do œrodka emitera).
@param[in] chunkIdx Indeks MemoryChunka do wpisania do bufora indeksów.
@param[in] triangleIdx Indeks trójk¹ta, który trafi do bufora indeksów.
@param[in] depthBuffer Bufor g³êbokoœci.
@param[in] indexBuffer Bufor indeksów.*/
void LightmapWorkerCPU::RasterizeTriangle( const Triangle4& triangle,
										   DirectX::XMFLOAT3* depths,
										   unsigned int chunkIdx,
										   unsigned int triangleIdx,
										   MemoryChunk& depthBuffer,
										   MemoryChunk& indexBuffer )
{
	XMFLOAT2A triangles[3];
	XMStoreFloat2A( &triangles[0], triangle.vertex1 );
	XMStoreFloat2A( &triangles[1], triangle.vertex2 );
	XMStoreFloat2A( &triangles[2], triangle.vertex3 );

	// Wyliczamy prostok¹t ograniczaj¹cy.
    int minX = min3( triangles[ 0 ].x, triangles[ 1 ].x, triangles[ 2 ].x);
    int minY = min3( triangles[ 0 ].y, triangles[ 1 ].y, triangles[ 2 ].y);
    int maxX = max3( triangles[ 0 ].x, triangles[ 1 ].x, triangles[ 2 ].x);
    int maxY = max3( triangles[ 0 ].y, triangles[ 1 ].y, triangles[ 2 ].y);

	// Iterujemy po wszystkich elementach prostok¹ta ograniczaj¹cego.
	// Sprawdzamy czy dany punkt nale¿y do trójk¹ta. Je¿eli nale¿y
	// zagl¹damy do z bufora.
	XMINT2 point;
	for( point.y = minY; point.y <= maxY; point.y++ )
	{
		for( point.x = minX; point.x <= maxX; point.x++ )
		{
            float w0 = BarycentricCoords( triangles[1], triangles[2], point );
            float w1 = BarycentricCoords( triangles[2], triangles[0], point );
            float w2 = BarycentricCoords( triangles[0], triangles[1], point );

			if( w0 >= 0 && w1 >= 0 && w2 >= 0 )	// Warunek na to, ¿e punkt jest wewn¹trz trójk¹ta.
			{
				float sum = w0 + w1 + w2;
				w0 = w0 / sum;
				w1 = w1 / sum;
				w2 = w2 / sum;

				int index = point.y * m_depthResolution + point.x;
				//float pointInverseDepth = w0 / ((float*)depths)[ 0 ] + w1 / ((float*)depths)[ 1 ] + w2 / ((float*)depths)[ 2 ];
				//float pointDepth = 1.0f / pointInverseDepth;
				float pointDepth = ((float*)depths)[ 0 ] * w0 + ((float*)depths)[ 1 ] * w1 + ((float*)depths)[ 2 ] * w2;
				float& bufferDepth = depthBuffer.Get<float>( index );
				if( bufferDepth > pointDepth && pointDepth > 0.0f )
				{
					BufferIndexing& indicies = indexBuffer.Get<BufferIndexing>( index );
					indicies.first = chunkIdx;
					indicies.second = triangleIdx;
					bufferDepth = pointDepth;
				}
			}
		}
	}

}

/**@brief Wylicza rzut punktu na p³aszczyznê.

@param[in] point Punkt wyra¿ony jako wektor znormalizowany od œrodka hemisfery do rzutowanego punktu.
@param[in] coordsSystem Opisuje uk³ad wspó³rzêdnych w jakim ma byæ wyra¿ony zwracany punkt.
@return Funkcja zwaraca dwuwymiarowy wektor oznaczaj¹cy punkt we wspó³rzednych w podanym
uk³adzie wspó³rzêdnych.*/
XMVECTOR LightmapWorkerCPU::ProjectPointToPlane( XMVECTOR point, const Triangle4& coordsSystem )
{
	XMVECTOR axisU = XMVector3Dot( coordsSystem.vertex2, point );
	XMVECTOR axisV = XMVector3Dot( coordsSystem.vertex3, point );
	XMVECTOR result = XMVectorMergeXY( axisU, axisV );

	return result;
}

/**@brief Uœrednia trzy normalne trójk¹ta.*/
XMVECTOR LightmapWorkerCPU::AverageNormal( const VertexFormat* triangle )
{
	XMVECTOR normal1 = XMLoadFloat3( &(*triangle).normal );		++triangle;
	XMVECTOR normal2 = XMLoadFloat3( &(*triangle).normal );		++triangle;
	XMVECTOR normal3 = XMLoadFloat3( &(*triangle).normal );

	return XMVectorDivide( XMVectorAdd( XMVectorAdd( normal1, normal2 ), normal3 ), XMVectorReplicate( 3.0f ) );
}

///**@brief Wylicza g³êbokoœæ dla poszczególnych wierzcho³ków trójk¹ta.*/
//DirectX::XMFLOAT3 LightmapWorkerCPU::ComputeDepths( const Triangle4& receiver, const Triangle4& emiter )
//{
//	XMVECTOR depth1 = XMVector3Length( XMVectorSubtract( receiver.vertex1, emiter.vertex1 ) );
//	XMVECTOR depth2 = XMVector3Length( XMVectorSubtract( receiver.vertex2, emiter.vertex2 ) );
//	XMVECTOR depth3 = XMVector3Length( XMVectorSubtract( receiver.vertex3, emiter.vertex3 ) );
//
//	XMFLOAT3 result;
//	XMStoreFloat( &result.x, depth1 );
//	XMStoreFloat( &result.y, depth2 );
//	XMStoreFloat( &result.z, depth3 );
//
//	return result;
//}

/**@brief Wylicza wspó³rzêdn¹ barycentryczn¹ punktu wzglêdem krawêdzie.
Wspó³rzêdne nie s¹ wyskalowane do jedynki. Nale¿y je wyskalowaæ kiedy siê wyliczy wszystkie.*/
float LightmapWorkerCPU::BarycentricCoords( DirectX::XMFLOAT2& vertex1, DirectX::XMFLOAT2& vertex2, DirectX::XMINT2& point )
{
	return (vertex2.x - vertex1.x) * ( point.y - vertex1.y ) - (vertex2.y - vertex1.y) * ( point.x - vertex1.x );
}

/**@brief Transformuje wierzcho³ki w przedziale [-1,1] do wspó³rzednych bufora g³êbokoœci.*/
void LightmapWorkerCPU::HemisphereViewport( Triangle4& receiver )
{
	XMVECTOR depthResolution = XMVectorReplicate( (float)m_depthResolution / 2.0f );
	receiver.vertex1 = XMVectorMultiplyAdd( receiver.vertex1, depthResolution, depthResolution );
	receiver.vertex2 = XMVectorMultiplyAdd( receiver.vertex2, depthResolution, depthResolution );
	receiver.vertex3 = XMVectorMultiplyAdd( receiver.vertex3, depthResolution, depthResolution );
}

