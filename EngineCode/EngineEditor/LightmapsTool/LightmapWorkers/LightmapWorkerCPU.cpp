#include "EngineEditor/LightmapsTool/stdafx.h"
#include "LightmapWorkerCPU.h"
#include "EngineCore/ModelsManager/DefaultAssets.h"

#include <cstring>

#include "Common/memory_leaks.h"

using namespace DirectX;

LightmapWorkerCPU::LightmapWorkerCPU( SceneData* sceneData )
	: LightmapWorker( sceneData )
{
	m_threshold = 0.001f;
}

// ============================================================================= //
// Helpers
// ============================================================================= //

/**@brief Szybsze (miejmy nadziejê) mno¿enie przez 3.*/
inline unsigned int mul3( unsigned int number )
{	return ( number << 1 ) + number;	}

/**@brief Dodaje pod podanymi indeksami w chunku element addValue.*/
inline XMFLOAT4& LoadAddStore( std::vector<MemoryChunk>& chunk, unsigned int i, unsigned int j, XMVECTOR addValue )
{
	auto lightValuePtr = &chunk[ i ].Get<XMFLOAT4>( j );
	XMVECTOR lightValue = XMLoadFloat4( lightValuePtr );
	lightValue = XMVectorAdd( addValue, lightValue );
	XMStoreFloat4( lightValuePtr, lightValue );
	
	return *lightValuePtr;
}


inline void CompareEmission( std::tuple<unsigned int, unsigned int, float>& prevEmission, unsigned int i, unsigned int j, const XMFLOAT4& newEmission )
{
	if( newEmission.x > std::get<2>( prevEmission ) )
		prevEmission = std::make_tuple( i, j, newEmission.x );
	if( newEmission.y > std::get<2>( prevEmission ) )
		prevEmission = std::make_tuple( i, j, newEmission.y );
	if( newEmission.z > std::get<2>( prevEmission ) )
		prevEmission = std::make_tuple( i, j, newEmission.z );
}

std::tuple<unsigned int, unsigned int, float> FindMaxEmision( std::vector<MemoryChunk>& emissionLight )
{
	std::tuple<unsigned int, unsigned int, float> emissionMax = std::make_tuple( 0, 0, -1.0f );
	for( unsigned int i = 0 ; i < emissionLight.size(); ++i )
		for( unsigned int j = 0; j < emissionLight[ i ].Count<XMFLOAT4>(); ++j )
		{
			auto& emissionVector = emissionLight[ i ].Get<XMFLOAT4>( j );
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
	BuildResult( emissionLight, reachedLight, verticies );

	m_lightmapState = LightmapState::GenerationEnded;		// Ustaw na koñcu funkcji, ¿eby w¹tek g³ówny móg³ zebraæ wyniki.
}

/**@brief Przygotowuje odpowiednie struktury do generacji.*/
void LightmapWorkerCPU::Prepare( std::vector<MemoryChunk>& emissionLight, std::vector<MemoryChunk>& reachedLight, std::vector<MemoryChunk>& verticies )
{
	auto& parts = m_data->objectParts;
	for( auto& part : parts )
	{
		unsigned int chunkSize = part.verticesCount * sizeof( XMFLOAT4 ) / 3;

		MemoryChunk emissionChunk( chunkSize );
		MemoryChunk reachedChunk( chunkSize );
		MemoryChunk verticiesChunk( part.verticesCount * sizeof( XMFLOAT3 ) );

		memset( reachedChunk.GetMemory<void>(), 0, chunkSize );
		memset( emissionChunk.GetMemory<void>(), 0, chunkSize );		// Potem wype³nimy wartoœciami emisji w pêtli.

		XMMATRIX transformMatrix = XMLoadFloat4x4( &part.transform );
		XMVector3TransformCoordStream( &verticiesChunk.Get<XMFLOAT3>( part.verticesCount ),
										sizeof( XMFLOAT3 ),
										&m_data->verticies[ part.chunkIdx ].Get<VertexNormalTexCord1>( part.bufferOffset ).position,
										sizeof( VertexNormalTexCord1 ), part.verticesCount, transformMatrix );

		if( part.emissive.x != 0.0f || part.emissive.y != 0.0f || part.emissive.z != 0.0f )
		{
			for( unsigned int k = 0; k < part.verticesCount / 3; ++k )
				emissionChunk.Get<XMFLOAT4>( part.verticesCount + k ) = part.emissive;
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
	std::tuple<unsigned int, unsigned int, float> emissionMax = FindMaxEmision( emissionLight );

	// Koñczymy generowanie, gdy najwiêksza zgromadzona w wielok¹cie energia spadnie poni¿ej pewnego poziomu.
	while( std::get<2>( emissionMax ) > m_threshold )
	{
		std::get<2>( emissionMax ) = -1.0f;
		Triangle4 emiter( &verticies[ std::get<0>( emissionMax )].Get<XMFLOAT3>( mul3( std::get<1>( emissionMax ) ) ) );

		// Trójk¹t opisany przez emissionMax emituje œwiat³o we wszystkich kierunkach.
		// Pierwsza implementacja nie sprawdza przys³aniania. Œwiat³o jest propagowane do wszystkich trójk¹tów.
		for( unsigned int i = 0 ; i < verticies.size(); ++i )
		{
			XMVECTOR materialDiffuse = XMLoadFloat4( &m_data->objectParts[i].diffuse );

			for( unsigned int j = 0; j < verticies[ i ].Count<Triangle3>(); ++j )
			{
				Triangle4 receiver( &verticies[ i ].Get<XMFLOAT3>( mul3( j ) ) );
				// Ta na prawdê to na razie pomijamy widocznoœæ licz¹c ten wspó³czynnik.
				XMVECTOR receivedLight = HemisphereRatio( emiter, receiver );
				receivedLight = XMVectorMultiply( materialDiffuse, receivedLight );

				LoadAddStore( reachedLight, i, j, receivedLight );
				const XMFLOAT4& emmisionPower = LoadAddStore( emissionLight, i, j, receivedLight );
				CompareEmission( emissionMax, i, j, emmisionPower );
			}
		}
	}
}

/**@brief Tworzy tablicê color - uv, która zostanie potem u¿yta do wyrenderowania lightmapy.*/
void LightmapWorkerCPU::BuildResult	( std::vector<MemoryChunk>& emisionLight, std::vector<MemoryChunk>& reachedLight, std::vector<MemoryChunk>& verticies )
{

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
	projectedTriangle.vertex1 = XMVectorDivide( receiver.vertex1, XMVector3Length( XMVectorSubtract( receiver.vertex1, emiterCenter ) ) );
	projectedTriangle.vertex2 = XMVectorDivide( receiver.vertex2, XMVector3Length( XMVectorSubtract( receiver.vertex2, emiterCenter ) ) );
	projectedTriangle.vertex3 = XMVectorDivide( receiver.vertex3, XMVector3Length( XMVectorSubtract( receiver.vertex3, emiterCenter ) ) );

	// Rzutujemy trójk¹t na p³aszczyznê okrêgu
	XMVECTOR emiterPlane = XMPlaneFromPoints( emiter.vertex1, emiter.vertex2, emiter.vertex3 );
		// W tym kroku robimy macierz rzutowania. Jest to równowa¿ne rzucaniu cienia przez punkty trójk¹ta i œwiat³o kierunkowe.
	XMMATRIX emiterPlaneProjection = XMMatrixShadow( emiterPlane, XMVectorSetW( XMVectorAdd( emiterCenter, emiterPlane ), 0.0 ) );
	
	projectedTriangle.vertex1 = XMVector3Transform( projectedTriangle.vertex1, emiterPlaneProjection );
	projectedTriangle.vertex2 = XMVector3Transform( projectedTriangle.vertex2, emiterPlaneProjection );
	projectedTriangle.vertex3 = XMVector3Transform( projectedTriangle.vertex3, emiterPlaneProjection );

	// Wyliczamy pole tego trójk¹ta, a potem porównujemy z polem ko³a o promieniu 1.
	XMVECTOR crossProduct = XMVector3Cross( XMVectorSubtract( projectedTriangle.vertex2, projectedTriangle.vertex1 ), XMVectorSubtract( projectedTriangle.vertex3, projectedTriangle.vertex1 ) );
	crossProduct = XMVector3Length( crossProduct );

	return XMVectorDivide( crossProduct, XMVectorReplicate( XM_2PI ) );
}


//void LightmapWorkerCPU::Prepare( std::vector<MemoryChunk>& emissionLight, std::vector<MemoryChunk>& reachedLight, std::vector<MemoryChunk>& verticies )
//{
//	auto& parts = m_data->objectParts;
//	auto object = parts[0].object;
//	unsigned int verticiesCount = 0;
//	unsigned int firstPartIndex = 0;
//
//	for( unsigned int i = 0; i < parts.size(); ++i )
//	{
//		if( parts[i].object != object )
//		{
//			verticiesCount = 0;
//
//			unsigned int chunkSize = verticiesCount * sizeof( XMFLOAT4 ) / 3;
//
//			MemoryChunk emissionChunk( chunkSize );
//			MemoryChunk reachedChunk( chunkSize );
//			MemoryChunk verticiesChunk( verticiesCount * sizeof( XMFLOAT3 ) );
//
//			memset( reachedChunk.GetMemory<void>(), 0, chunkSize );
//			memset( emissionChunk.GetMemory<void>(), 0, chunkSize );		// Potem wype³nimy wartoœciami emisji w pêtli.
//
//			for( unsigned int j = firstPartIndex; j < i; ++j, verticiesCount += parts[j].verticesCount )
//			{
//				XMMATRIX transformMatrix = XMLoadFloat4x4( &parts[j].transform );
//				XMVector3TransformCoordStream( &verticiesChunk.Get<XMFLOAT3>( verticiesCount ),
//											   sizeof( XMFLOAT3 ),
//											   &m_data->verticies[ parts[j].chunkIdx ].Get<VertexNormalTexCord1>( parts[j].bufferOffset ).position,
//											   sizeof( VertexNormalTexCord1 ), parts[j].verticesCount, transformMatrix );
//
//				if( parts[j].emissive.x != 0.0f || parts[j].emissive.y != 0.0f || parts[j].emissive.z != 0.0f )
//				{
//					for( unsigned int k = 0; k < parts[j].verticesCount / 3; ++k )
//						emissionChunk.Get<XMFLOAT4>( verticiesCount + k ) = parts[j].emissive;
//				}
//			}
//
//			emissionLight.push_back( std::move( emissionChunk ) );
//			reachedLight.push_back( std::move( reachedChunk ) );
//			verticies.push_back( std::move( verticiesChunk ) );
//
//			// Zakoñczyliœmy przetwarzanie obiektu przechodzimy do nastêpnego.
//			object = parts[i].object;
//			firstPartIndex = i;
//			verticiesCount = 0;
//		}
//		else
//			verticiesCount += parts[i].verticesCount;
//	}
//}
