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

/**@brief Szybsze (miejmy nadziej�) mno�enie przez 3.*/
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

/**@brief G��wna funkcja generuj�ca lightmapy. Zaimplementuj w klasie pochodnej.*/
void LightmapWorkerCPU::Generate		()
{
	if( m_data->objectParts.empty() )
		return;

	// Chunki dla ca�ych obiek�w (nie dla podcz�ci)
	std::vector<MemoryChunk> emissionLight;
	std::vector<MemoryChunk> reachedLight;
	std::vector<MemoryChunk> verticies;			// Przekszta�cone wierzcho�ki. Mo�e potem dojd� normalne.

	Prepare( emissionLight, reachedLight, verticies );
	Radiosity( emissionLight, reachedLight, verticies );
	BuildResult( emissionLight, reachedLight, verticies );

	m_lightmapState = LightmapState::GenerationEnded;		// Ustaw na ko�cu funkcji, �eby w�tek g��wny m�g� zebra� wyniki.
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
		memset( emissionChunk.GetMemory<void>(), 0, chunkSize );		// Potem wype�nimy warto�ciami emisji w p�tli.

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
	// Identyfikuje wielok�t z najwi�ksz� energi� do wyemitowania.
	std::tuple<unsigned int, unsigned int, float> emissionMax = FindMaxEmision( emissionLight );

	// Ko�czymy generowanie, gdy najwi�ksza zgromadzona w wielok�cie energia spadnie poni�ej pewnego poziomu.
	while( std::get<2>( emissionMax ) > m_threshold )
	{
		std::get<2>( emissionMax ) = -1.0f;
		Triangle4 emiter( &verticies[ std::get<0>( emissionMax )].Get<XMFLOAT3>( mul3( std::get<1>( emissionMax ) ) ) );

		// Tr�jk�t opisany przez emissionMax emituje �wiat�o we wszystkich kierunkach.
		// Pierwsza implementacja nie sprawdza przys�aniania. �wiat�o jest propagowane do wszystkich tr�jk�t�w.
		for( unsigned int i = 0 ; i < verticies.size(); ++i )
		{
			XMVECTOR materialDiffuse = XMLoadFloat4( &m_data->objectParts[i].diffuse );

			for( unsigned int j = 0; j < verticies[ i ].Count<Triangle3>(); ++j )
			{
				Triangle4 receiver( &verticies[ i ].Get<XMFLOAT3>( mul3( j ) ) );
				// Ta na prawd� to na razie pomijamy widoczno�� licz�c ten wsp�czynnik.
				XMVECTOR receivedLight = HemisphereRatio( emiter, receiver );
				receivedLight = XMVectorMultiply( materialDiffuse, receivedLight );

				LoadAddStore( reachedLight, i, j, receivedLight );
				const XMFLOAT4& emmisionPower = LoadAddStore( emissionLight, i, j, receivedLight );
				CompareEmission( emissionMax, i, j, emmisionPower );
			}
		}
	}
}

/**@brief Tworzy tablic� color - uv, kt�ra zostanie potem u�yta do wyrenderowania lightmapy.*/
void LightmapWorkerCPU::BuildResult	( std::vector<MemoryChunk>& emisionLight, std::vector<MemoryChunk>& reachedLight, std::vector<MemoryChunk>& verticies )
{

}

/**@brief Wylicza dla emitera i odbiornika wsp�czynnik widoczno�ci w oparciu o hemisfer�.*/
XMVECTOR LightmapWorkerCPU::HemisphereRatio( Triangle4& emiter, Triangle4& receiver )
{
	// �rodek ci�ko�ci tr�jk�ta przyjujemy za �rodek emitera.
	XMVECTOR emiterCenter = XMVectorAdd( emiter.vertex1, emiter.vertex2 );
	emiterCenter = XMVectorAdd( emiterCenter, emiter.vertex3 );
	emiterCenter = XMVectorDivide( emiterCenter, XMVectorReplicate( 3.0f ) );

	// Rzutujemy wierzcho�ki receivera na p�sfer� jednostkow�. Oznacza to podzielenie ka�dego z wektor�w
	// Receivera przez d�ugo�� wektora emiterCenter - receiver.vertexN. (Chcemy dosta� wektor o d�ugo�ci 1)
	Triangle4 projectedTriangle;
	projectedTriangle.vertex1 = XMVectorDivide( receiver.vertex1, XMVector3Length( XMVectorSubtract( receiver.vertex1, emiterCenter ) ) );
	projectedTriangle.vertex2 = XMVectorDivide( receiver.vertex2, XMVector3Length( XMVectorSubtract( receiver.vertex2, emiterCenter ) ) );
	projectedTriangle.vertex3 = XMVectorDivide( receiver.vertex3, XMVector3Length( XMVectorSubtract( receiver.vertex3, emiterCenter ) ) );

	// Rzutujemy tr�jk�t na p�aszczyzn� okr�gu
	XMVECTOR emiterPlane = XMPlaneFromPoints( emiter.vertex1, emiter.vertex2, emiter.vertex3 );
		// W tym kroku robimy macierz rzutowania. Jest to r�wnowa�ne rzucaniu cienia przez punkty tr�jk�ta i �wiat�o kierunkowe.
	XMMATRIX emiterPlaneProjection = XMMatrixShadow( emiterPlane, XMVectorSetW( XMVectorAdd( emiterCenter, emiterPlane ), 0.0 ) );
	
	projectedTriangle.vertex1 = XMVector3Transform( projectedTriangle.vertex1, emiterPlaneProjection );
	projectedTriangle.vertex2 = XMVector3Transform( projectedTriangle.vertex2, emiterPlaneProjection );
	projectedTriangle.vertex3 = XMVector3Transform( projectedTriangle.vertex3, emiterPlaneProjection );

	// Wyliczamy pole tego tr�jk�ta, a potem por�wnujemy z polem ko�a o promieniu 1.
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
//			memset( emissionChunk.GetMemory<void>(), 0, chunkSize );		// Potem wype�nimy warto�ciami emisji w p�tli.
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
//			// Zako�czyli�my przetwarzanie obiektu przechodzimy do nast�pnego.
//			object = parts[i].object;
//			firstPartIndex = i;
//			verticiesCount = 0;
//		}
//		else
//			verticiesCount += parts[i].verticesCount;
//	}
//}
