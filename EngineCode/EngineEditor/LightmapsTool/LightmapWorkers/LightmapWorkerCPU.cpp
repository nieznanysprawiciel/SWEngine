#include "EngineEditor/LightmapsTool/stdafx.h"
#include "LightmapWorkerCPU.h"
#include "EngineCore/ModelsManager/DefaultAssets.h"

#include <cstring>

#include "Common/memory_leaks.h"

using namespace DirectX;

LightmapWorkerCPU::LightmapWorkerCPU( SceneData* sceneData )
	: LightmapWorker( sceneData )
{
	m_threshold = 0.01f;
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
	auto lightValuePtr = &chunk[ i ].Get<XMFLOAT3>( j );
	XMVECTOR lightValue = XMLoadFloat3( lightValuePtr );
	lightValue = XMVectorAdd( addValue, lightValue );
	XMStoreFloat3( lightValuePtr, lightValue );
	
	return *lightValuePtr;
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
	std::vector<MemoryChunk> verticies;			// Przekszta³cone wierzcho³ki. Mo¿e potem dojd¹ normalne.

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
		XMVector3TransformCoordStream( &verticiesChunk.Get<VertexFormat>( 0 ).position, sizeof( VertexFormat ),
										&m_data->verticies[ part.chunkIdx ].Get<VertexNormalTexCord1>( part.bufferOffset ).position,
										sizeof( VertexNormalTexCord1 ), part.verticesCount, transformMatrix );

		// Transform normals
		XMVector3TransformNormalStream( &verticiesChunk.Get<VertexFormat>( 0 ).normal, sizeof( VertexFormat ),
										&m_data->verticies[ part.chunkIdx ].Get<VertexNormalTexCord1>( part.bufferOffset ).normal,
										sizeof( VertexNormalTexCord1 ), part.verticesCount, transformMatrix );

		if( part.emissive.x != 0.0f || part.emissive.y != 0.0f || part.emissive.z != 0.0f )
		{
			XMFLOAT3 materialEmissive( part.emissive.x, part.emissive.y, part.emissive.z );
			for( unsigned int k = 0; k < part.verticesCount / 3; ++k )
				emissionChunk.Get<XMFLOAT3>( k ) = materialEmissive;
		}

		emissionLight.push_back( std::move( emissionChunk ) );
		reachedLight.push_back( std::move( reachedChunk ) );
		verticies.push_back( std::move( verticiesChunk ) );
	}
}


/**@brief Wykonuje algorytm radiosity.*/
void LightmapWorkerCPU::Radiosity( std::vector<MemoryChunk>& emissionLight, std::vector<MemoryChunk>& reachedLight, std::vector<MemoryChunk>& verticies )
{
	// Identyfikuje wielok¹t z najwiêksz¹ energi¹ do wyemitowania.
	std::tuple<unsigned int, unsigned int, float> emissionMax = FindMaxEmission( emissionLight );

	// Koñczymy generowanie, gdy najwiêksza zgromadzona w wielok¹cie energia spadnie poni¿ej pewnego poziomu.
	while( std::get<2>( emissionMax ) > m_threshold )
	{
		std::get<2>( emissionMax ) = 0.0f;
		unsigned int idx1 = std::get<0>( emissionMax );
		unsigned int idx2 = std::get<1>( emissionMax );

		Triangle4 emiterPosition( &verticies[ idx1 ].Get<VertexFormat>( mul3( idx2 ) ) );
		XMVECTOR emiterNormal = AverageNormal( &verticies[ idx1 ].Get<VertexFormat>( mul3( idx2 ) ) );

		XMVECTOR emitedLight = XMLoadFloat3( &emissionLight[ idx1 ].Get<XMFLOAT3>( idx2 ) );
		XMStoreFloat3( &emissionLight[ idx1 ].Get<XMFLOAT3>( idx2 ), XMVectorZero() );

		// Trójk¹t opisany przez emissionMax emituje œwiat³o we wszystkich kierunkach.
		// Pierwsza implementacja nie sprawdza przys³aniania. Œwiat³o jest propagowane do wszystkich trójk¹tów.
		for( unsigned int i = 0 ; i < emissionLight.size(); ++i )
		{
			XMVECTOR materialDiffuse = XMLoadFloat4( &m_data->objectParts[i].diffuse );

			for( unsigned int j = 0; j < emissionLight[ i ].Count<XMFLOAT3>(); ++j )
			{
				if( idx2 == j && idx1 == i )
					continue;

				XMVECTOR receiverNormal = AverageNormal( &verticies[ i ].Get<VertexFormat>( mul3( j ) ) );
				XMVECTOR normalsDot = XMVector3Dot( emiterNormal, receiverNormal );
				if( XMVector3Greater( normalsDot, XMVectorZero() ) )
					continue;		// Trójk¹ty musz¹ byæ zwrócone w przeciwn¹ stronê.

				Triangle4 receiverPosition( &verticies[ i ].Get<VertexFormat>( mul3( j ) ) );
				
				// Ta na prawdê to na razie pomijamy widocznoœæ licz¹c ten wspó³czynnik.
				XMVECTOR receivedLight = HemisphereRatio( emiterPosition, receiverPosition );
				if( XMVector3Greater( receivedLight, XMVectorReplicate( 1.0f ) ) )
				{
					XMVector3Greater( receivedLight, XMVectorReplicate( 1.0f ) );
				}

				receivedLight = XMVectorMultiply( emitedLight, receivedLight );
				receivedLight = XMVectorMultiply( materialDiffuse, receivedLight );

				const XMFLOAT3& emmisionPower = LoadAddStore( emissionLight, i, j, receivedLight );
				LoadAddStore( reachedLight, i, j, receivedLight );
				CompareEmission( emissionMax, i, j, emmisionPower );
			}
		}
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
		// Sumujemy liczbê wierzcho³ków w obiekcie. Dopiero jak zacznie siê nowy obiekt to tworzymy bufor.
		if( parts[i].object != object )
		{			
			MemoryChunk colorMap( verticiesCount * sizeof( CoordColor ) );

			unsigned int verticiesOffset = 0;
			for( unsigned int j = firstPartIndex; j < i; ++j, verticiesOffset += parts[j].verticesCount )
			{
				for( unsigned int k = 0; k < reachedLight[ j ].Count<XMFLOAT3>(); ++k )
				{
					CoordColor& colorVertex = colorMap.Get<CoordColor>( verticiesOffset + mul3( k ) );		// Jest 3 razy wiêcej wierzcho³ków ni¿ kolorów.
					XMFLOAT3& lightColor = reachedLight[ j ].Get<XMFLOAT3>( k );
					colorVertex.color = lightColor;
					colorVertex.texCoords = m_data->verticies[ parts[j].chunkIdx ].Get<VertexNormalTexCord1>( parts[j].bufferOffset + mul3( k ) ).tex_cords;

					colorVertex = colorMap.Get<CoordColor>( verticiesOffset + mul3( k ) + 1 );
					colorVertex.color = lightColor;
					colorVertex.texCoords = m_data->verticies[ parts[j].chunkIdx ].Get<VertexNormalTexCord1>( parts[j].bufferOffset + mul3( k ) + 1 ).tex_cords;

					colorVertex = colorMap.Get<CoordColor>( verticiesOffset + mul3( k ) + 2 );
					colorVertex.color = lightColor;
					colorVertex.texCoords = m_data->verticies[ parts[j].chunkIdx ].Get<VertexNormalTexCord1>( parts[j].bufferOffset + mul3( k ) + 2 ).tex_cords;
				}
			}

			m_resultData.push_back( std::move( colorMap ) );

			// Zakoñczyliœmy przetwarzanie obiektu przechodzimy do nastêpnego.
			object = parts[i].object;
			firstPartIndex = i;
			verticiesCount = 0;
		}
		else
			verticiesCount += parts[i].verticesCount;
	}
}

/**@brief Wylicza dla emitera i odbiornika wspó³czynnik widocznoœci w oparciu o hemisferê.*/
XMVECTOR LightmapWorkerCPU::HemisphereRatio( Triangle4& emiter, Triangle4& receiver )
{
	// Œrodek ciê¿koœci trójk¹ta przyjujemy za œrodek emitera.
	XMVECTOR emiterCenter = XMVectorAdd( emiter.vertex1, emiter.vertex2 );
	emiterCenter = XMVectorAdd( emiterCenter, emiter.vertex3 );
	emiterCenter = XMVectorDivide( emiterCenter, XMVectorReplicate( 3.0f ) );

	// Rzutujemy wierzcho³ki receivera na pó³sferê jednostkow¹. Oznacza to podzielenie ka¿dego z wektorów
	// Receivera przez d³ugoœæ wektora emiterCenter - receiver.vertexN. (Chcemy dostaæ wektor o d³ugoœci 1)
	Triangle4 projectedTriangle;
	XMVECTOR centerToVertex1 = XMVectorSubtract( receiver.vertex1, emiterCenter );
	XMVECTOR centerToVertex2 = XMVectorSubtract( receiver.vertex2, emiterCenter );
	XMVECTOR centerToVertex3 = XMVectorSubtract( receiver.vertex3, emiterCenter );

	projectedTriangle.vertex1 = XMVectorAdd( emiterCenter, XMVector3Normalize( centerToVertex1 ) );
	projectedTriangle.vertex2 = XMVectorAdd( emiterCenter, XMVector3Normalize( centerToVertex2 ) );
	projectedTriangle.vertex3 = XMVectorAdd( emiterCenter, XMVector3Normalize( centerToVertex3 ) );

	// Rzutujemy trójk¹t na p³aszczyznê okrêgu
	XMVECTOR emiterPlane = XMPlaneFromPoints( emiter.vertex1, emiter.vertex2, emiter.vertex3 );

	projectedTriangle.vertex1 = ProjectPointToPlane( emiterPlane, emiterCenter, projectedTriangle.vertex1 );
	projectedTriangle.vertex2 = ProjectPointToPlane( emiterPlane, emiterCenter, projectedTriangle.vertex2 );
	projectedTriangle.vertex3 = ProjectPointToPlane( emiterPlane, emiterCenter, projectedTriangle.vertex3 );

	// Wyliczamy pole tego trójk¹ta, a potem porównujemy z polem ko³a o promieniu 1.
	XMVECTOR sideA = XMVectorSubtract( projectedTriangle.vertex2, projectedTriangle.vertex1 );
	XMVECTOR sideB = XMVectorSubtract( projectedTriangle.vertex3, projectedTriangle.vertex1 );

	XMVECTOR crossProduct = XMVector3Cross( sideA, sideB );
	crossProduct = XMVector3Length( crossProduct );

	return XMVectorDivide( crossProduct, XMVectorReplicate( XM_2PI ) );
}


XMVECTOR LightmapWorkerCPU::ProjectPointToPlane( XMVECTOR plane, XMVECTOR planePoint, XMVECTOR point )
{
	XMVECTOR pointToPlanePoint = XMVectorSubtract( planePoint, point );
	XMVECTOR dotProduct = XMVector3Dot( XMVector3Normalize( plane ), pointToPlanePoint );
	return XMVectorSubtract( point, XMVectorMultiply( plane, dotProduct ) );
}

XMVECTOR LightmapWorkerCPU::AverageNormal( const VertexFormat* triangle )
{
	XMVECTOR normal1 = XMLoadFloat3( &(*triangle).normal );		++triangle;
	XMVECTOR normal2 = XMLoadFloat3( &(*triangle).normal );		++triangle;
	XMVECTOR normal3 = XMLoadFloat3( &(*triangle).normal );

	return XMVectorDivide( XMVectorAdd( XMVectorAdd( normal1, normal2 ), normal3 ), XMVectorReplicate( 3.0f ) );
}

