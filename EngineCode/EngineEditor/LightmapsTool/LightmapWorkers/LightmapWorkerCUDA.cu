#include "EngineEditor/LightmapsTool/stdafx.h"

// To makro musi byæ zdefiniowane przed zaincludowaniem DirectXMath (w LightmapWorkerCUDA.h).
// Biblioteka u¿ywa domyœlnie zestawu instrukcji SSE2 i trzeba to wy³¹czyæ.
#define _XM_NO_INTRINSICS_

#include "EngineEditor/LightmapsTool/LightmapWorkers/LightmapWorkerCUDA.h"
#include "EngineCore/ModelsManager/DefaultAssets.h"

#include <cstring>
#include <limits>
// Trzeba oddefiniowaæ, je¿eli chce siê korzystaæ z numeric_limits
#undef max

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <thrust/functional.h>
#include <thrust/random.h>
#include <thrust/device_vector.h>
#include <thrust/sequence.h>
#include <thrust/reduce.h>
#include <thrust/transform_reduce.h>
#include <thrust/iterator/counting_iterator.h>
#include <thrust/iterator/zip_iterator.h>

#include <cuda.h>
#include "helper_cuda.h"

//#include "Common/memory_leaks.h"

using namespace DirectX;

LightmapWorkerCUDA::LightmapWorkerCUDA( SceneData* sceneData )
	: LightmapWorker( sceneData )
{
	m_threshold = 0.04f;
}

// ============================================================================= //
// Funkcje pomocnicze - ogólne
// ============================================================================= //


/**@brief Pobiera dane o wierzcho³kach zgromadzone na karcie graficznej.

@todo Funkcja zostanie zaimplementowana w przysz³oœci.

Dane o wierzcho³kach meshy s¹ zgromadzone ju¿ na karcie graficznej.
Ta funkcja ma umo¿liwiæ pobranie wskaŸników na te dane z obiektu BufferObject, który
S³u¿y opakowaniu obiektów DirectXowych (lub w przysz³oœci tak¿e innych API graficznych).

Na razie dla spójnoœci kodu jest zwracany obiekt thrust::device_vector<VertexNormalTexCord1>.
W przysz³oœci bêdzie to prawdopodobnie go³y wskaŸnik na pamiêæ i resztê aplikacje bêdzie trzeba dostosowaæ.

Do pobierania wskaŸników pos³u¿¹ funkcje:
cudaGraphicsMapResources
cudaGraphicsResourceGetMappedPointer*/
thrust::device_vector<VertexNormalTexCord1> GetVerticiesFromGraphicAPI( BufferObject* buffer )
{
	return thrust::device_vector<VertexNormalTexCord1>();
}

/**@brief Zmapowane wierzcho³ki funkcj¹ GetVerticiesFromGraphicAPI trzeba "odmapowaæ" po zakoñczeniu przetwarzania.*/
void UnmapVerticiesFromGraphicAPI()
{

}


/**@brief Szybsze (miejmy nadziejê) mno¿enie przez 3.*/
inline unsigned int mul3( unsigned int number )
{
	return ( number << 1 ) + number;
}

inline float fmax( float a, float b, float c )
{
	return fmax( a, fmax( b, c ) );
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
// Funkcje pomocnicze - graficzne
// ============================================================================= //

/**@brief Funkcja znajduje trojk¹t o najwiêkszej zgromadznej energii do wyemitowania.

Prawdopodobnie bêdzie to robione jak¹œ form¹ redukcji.*/
__host__ std::tuple<unsigned int, unsigned int, float> FindMaxEmission( thrust::device_vector<glm::vec3>& emissionLight, unsigned emissionLightSize )
{
	//typedef std::tuple<unsigned int, unsigned int, glm::vec3> Emission;

	//class CompareEmissionFunction : thrust::binary_function < Emission, Emission, Emission >
	//{
	//public:
	//	Emission operator()( Emission &left, Emission &right )
	//	{
	//		glm::vec3 leftEmission = std::get<2>( left );
	//		glm::vec3 rightEmission = std::get<2>( right );

	//		float maxL = fmax( leftEmission.x, leftEmission.y, leftEmission.z );
	//		float maxR = fmax( rightEmission.x, rightEmission.y, rightEmission.z );

	//		return ( maxL > maxR ) ? left : right;
	//	}
	//};

	//Emission initialValue = std::make_tuple( 0, 0, glm::vec3( -1.0f, -1.0f, -1.0f ) );
	//Emission foundValue = thrust::reduce( emissionLight, emissionLight + emissionLightSize, initialValue, CompareEmissionFunction() );

	//glm::vec3 emissionValue = std::get<2>( foundValue );
	//return std::make_tuple( std::get<0>( foundValue ), std::get<1>( foundValue ), fmax( emissionValue.x, emissionValue.y, emissionValue.z ) );
}

/**@brief Wylicza wspó³rzêdn¹ barycentryczn¹ punktu wzglêdem krawêdzie.
Wspó³rzêdne nie s¹ wyskalowane do jedynki. Nale¿y je wyskalowaæ kiedy siê wyliczy wszystkie.*/
__device__ static float BarycentricCoords( glm::vec2& vertex1, glm::vec2& vertex2, glm::ivec2& point )
{
	return ( vertex2.x - vertex1.x ) * ( point.y - vertex1.y ) - ( vertex2.y - vertex1.y ) * ( point.x - vertex1.x );
}

/*
@param[inout] receiver Trójk¹t do zrzutowania.W tej zmiennej zwracany jest trójk¹t wynikowy.
@return Zwraca wektor z g³êbokoœciami dla ka¿dego trójk¹ta.Je¿eli jakiœ wierzcho³ek
znajduje siê za p³aszczyzn¹ rzutowania, funkcja zwraca(-1.0, -1.0, -1.0).W normalnej sytuacji
wszystkie wspó³rzêdne s¹ dodatnie. */
__device__ static glm::vec3 HemisphereCast( Triangle4& emiter, Triangle4& receiver, glm::mat4& emiterViewMatrix )
{
	// Transformujemy wierzcho³ki do uk³adu emitera. Dziêki temu xy jest pozycj¹ na p³aszczyŸnie (na razie jeszcze nie jednostkowej)
	// a z jest odleg³oœci¹ od emitera.
	// @todo Zastanowiæ sie czy nie da siê tu mno¿yæ przez macierz 3x3.
	glm::vec4 centerToVertex1 = emiterViewMatrix * glm::vec4( receiver.vertex1, 1.0f );		// Poniewa¿ to s¹ wierzcho³ki, to ustawiamy komponent w na 1.
	glm::vec4 centerToVertex2 = emiterViewMatrix * glm::vec4( receiver.vertex2, 1.0f );
	glm::vec4 centerToVertex3 = emiterViewMatrix * glm::vec4( receiver.vertex3, 1.0f );

	// Wyliczenie d³ugoœci od œrodka do wierzcho³ków.
	float depth1 = glm::length( glm::vec3( centerToVertex1 ) );
	float depth2 = glm::length( glm::vec3( centerToVertex2 ) );
	float depth3 = glm::length( glm::vec3( centerToVertex3 ) );

	// Normalizacja wektorów
	receiver.vertex1 = glm::vec3( centerToVertex1 / depth1 );
	receiver.vertex2 = glm::vec3( centerToVertex2 / depth2 );
	receiver.vertex3 = glm::vec3( centerToVertex3 / depth3 );

	// Po tym przekszta³ceniu wektor jest wyra¿ony we wspó³rzêdnych p³aszczyzny. Sk³adowa z oznacza
	// odleg³oœæ obiektu od p³aszczyzny. Uwaga! Nie jest to odleg³oœæ od œrodka p³aszczyzny, ale
	// odleg³oœæ rzutu prostopad³ego na tê p³aszczyznê. Mimo wszystko taka g³êbokoœæ pozwala na poprawne
	// wyznaczenie przys³aniania.
	glm::vec3 depthVector;
	depthVector.x = -centerToVertex1.z;
	depthVector.y = -centerToVertex2.z;
	depthVector.z = -centerToVertex3.z;

	return depthVector;
}

/**@brief Dodaje pod podanymi indeksami w chunku element addValue.

Do zaimplementowania*/
__device__ inline glm::vec3& LoadAddStore( glm::vec3* chunk, unsigned int i, unsigned int j, glm::vec4 addValue )
{
	//auto& lightValueRef = chunk[ i ].Get<glm::vec3>( j );
	//glm::vec4 lightValue = XMLoadFloat3( &lightValueRef );
	//lightValue = XMVectorAdd( addValue, lightValue );
	//XMStoreFloat3( &lightValueRef, lightValue );

	//return lightValueRef;
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
__device__ static void RasterizeTriangle( const Triangle4& triangle,
										  glm::vec3* depths,
										  unsigned int chunkIdx,
										  unsigned int triangleIdx,
										  float* depthBuffer,
										  BufferIndexing* indexBuffer,
										  unsigned int m_depthResolution )
{
	glm::vec2 triangles[ 3 ];

	triangles[ 0 ].x = triangle.vertex1.x;
	triangles[ 0 ].y = triangle.vertex1.y;

	triangles[ 1 ].x = triangle.vertex2.x;
	triangles[ 1 ].y = triangle.vertex2.y;

	triangles[ 2 ].x = triangle.vertex3.x;
	triangles[ 2 ].y = triangle.vertex3.y;

	// Wyliczamy prostok¹t ograniczaj¹cy.
	int minX = min3( triangles[ 0 ].x, triangles[ 1 ].x, triangles[ 2 ].x );
	int minY = min3( triangles[ 0 ].y, triangles[ 1 ].y, triangles[ 2 ].y );
	int maxX = max3( triangles[ 0 ].x, triangles[ 1 ].x, triangles[ 2 ].x );
	int maxY = max3( triangles[ 0 ].y, triangles[ 1 ].y, triangles[ 2 ].y );

	// Iterujemy po wszystkich elementach prostok¹ta ograniczaj¹cego.
	// Sprawdzamy czy dany punkt nale¿y do trójk¹ta. Je¿eli nale¿y
	// zagl¹damy do z bufora.
	glm::ivec2 point;
	for( point.y = minY; point.y <= maxY; point.y++ )
	{
		for( point.x = minX; point.x <= maxX; point.x++ )
		{
			float w0 = BarycentricCoords( triangles[ 1 ], triangles[ 2 ], point );
			float w1 = BarycentricCoords( triangles[ 2 ], triangles[ 0 ], point );
			float w2 = BarycentricCoords( triangles[ 0 ], triangles[ 1 ], point );

			if( w0 >= 0 && w1 >= 0 && w2 >= 0 )	// Warunek na to, ¿e punkt jest wewn¹trz trójk¹ta.
			{
				float sum = w0 + w1 + w2;
				w0 = w0 / sum;
				w1 = w1 / sum;
				w2 = w2 / sum;

				int index = point.y * m_depthResolution + point.x;
				//float pointInverseDepth = w0 / ((float*)depths)[ 0 ] + w1 / ((float*)depths)[ 1 ] + w2 / ((float*)depths)[ 2 ];
				//float pointDepth = 1.0f / pointInverseDepth;
				float pointDepth = ( (float*)depths )[ 0 ] * w0 + ( (float*)depths )[ 1 ] * w1 + ( (float*)depths )[ 2 ] * w2;
				float& bufferDepth = depthBuffer[ index ];
				if( bufferDepth > pointDepth && pointDepth > 0.0f )
				{
					BufferIndexing& indicies = indexBuffer[ index ];
					indicies.first = chunkIdx;
					indicies.second = triangleIdx;
					bufferDepth = pointDepth;
				}
			}
		}
	}

}

/**@brief Transformuje wierzcho³ki w przedziale [-1,1] do wspó³rzednych bufora g³êbokoœci.*/
__device__ static void HemisphereViewport( Triangle4& receiver, unsigned int m_depthResolution )
{
	glm::vec3 depthResolution = glm::vec3( (float)m_depthResolution / 2.0f );
	
	receiver.vertex1 = receiver.vertex1 * depthResolution + depthResolution;
	receiver.vertex2 = receiver.vertex2 * depthResolution + depthResolution;
	receiver.vertex3 = receiver.vertex3 * depthResolution + depthResolution;
}

/**@brief Tworzy macierz przekszta³caj¹c¹ wierzcho³ki do uk³adu odniesienia emitera.*/
__host__ glm::mat4 LightmapWorkerCUDA::EmiterViewMatrix( Triangle4& emiter )
{
	// Œrodek ciê¿koœci trójk¹ta przyjujemy za œrodek emitera.
	glm::vec3 emiterCenter = emiter.vertex1 + emiter.vertex2;
	emiterCenter = emiterCenter + emiter.vertex3;
	emiterCenter = emiterCenter + glm::vec3( 3.0f );

	// Wektor prostopad³y do p³aszczyzny, na której le¿¹ trójk¹ty.
	glm::vec3 edge12 = emiter.vertex2 - emiter.vertex1;
	glm::vec3 edge13 = emiter.vertex3 - emiter.vertex1;
	glm::vec3 normal = glm::normalize( glm::cross( edge12, edge13 ) );

	// Ustalamy dowolnie (byle ortogonalnie) kierunek "w górê".
	glm::vec3 upDirection = glm::normalize( glm::cross( edge12, normal ) );

	// Tworzymy macierz widoku.
	glm::mat4 viewMatrix = glm::lookAt( emiterCenter, normal, upDirection );

	return viewMatrix;
}

// ============================================================================= //
// Generation - kernele
// ============================================================================= //

// @todo Zast¹piæ lepszym rozwi¹zaniem ni¿ zmienna globalna.
unsigned int depthBufferSize = 0;

/**@brief Czyœci bufory g³êbokoœci i indeksów.*/
__global__ static void KernelClearDepthIndex( float *depthBuffer, BufferIndexing *indexBuffer )
{
	int i = blockIdx.x * blockDim.x + threadIdx.x;

	depthBuffer[ i ] = std::numeric_limits<float>::max();
	indexBuffer[ i ].first = INVALID_INDEX;
	indexBuffer[ i ].second = INVALID_INDEX;
}

__global__ static void KernelDepthPass( VertexFormat* verticies, Size vertexMax, uint16 i,
										 float* depthBuffer, BufferIndexing* indexBuffer, Triangle4 emiterPosition, glm::mat4 emiterViewMatrix )
{
	// Pierwsza wspó³rzêdna wierzcho³ka
	int j = blockIdx.x * blockDim.x + threadIdx.x;
	if( j > vertexMax ) return;


	// Wyliczamy pozycjê odbiorcy w uk³adzie wspó³rzêdnych bufora g³êbokoœci.
	Triangle4 receiverPosition( verticies + j );
	glm::vec3 receiverDepths = HemisphereCast( emiterPosition, receiverPosition, emiterViewMatrix );

	if( receiverDepths.x < 0.0f || receiverDepths.y < 0.0f || receiverDepths.z < 0.0f )
		return;  // Trójk¹t jest czêœciowo za p³aszczyzn¹. Musimy go odrzuciæ.

	HemisphereViewport( receiverPosition, depthBufferSize );
	RasterizeTriangle( receiverPosition, &receiverDepths, i, j, depthBuffer, indexBuffer, depthBufferSize );
}


#define MAX_MATERIALS 300
__constant__ glm::vec3 materials[ MAX_MATERIALS ];

__global__ static void KernelTransferPass( BufferIndexing *indexBuffer,
											glm::vec3* reachedLight, glm::vec3* emissionLight, glm::vec4 emitedLight )
{
	int i = blockIdx.x * blockDim.x + threadIdx.x;

	BufferIndexing indicies = indexBuffer[ i ];
	// Tylko piksele na kole powinny dostaæ dawkê energii, a bufor jest kwadratem.
	// Dlatego elementy, na które nie zosta³ zrzutowany ¿aden trójk¹t zosta³y zainicjowane wartoœci¹ INVALID_INDEX.
	if( indicies.first != INVALID_INDEX )
	{
		glm::vec3 materialDiffuse = materials[ indicies.first ];
		glm::vec3 receivedLight = materialDiffuse * glm::vec3( emitedLight );

		const glm::vec3& emissionPower = LoadAddStore( emissionLight, indicies.first, indicies.second, receivedLight );
		LoadAddStore( reachedLight, indicies.first, indicies.second, receivedLight );
	}
}

// ============================================================================= //
// Generation
// ============================================================================= //

/**@brief G³ówna funkcja generuj¹ca lightmapy. Zaimplementuj w klasie pochodnej.*/
void LightmapWorkerCUDA::Generate()
{
	if( m_data->objectParts.empty() )
		return;

	checkCudaErrors( cudaSetDevice( 0 ) );


	//// Chunki dla ca³ych obieków (nie dla podczêœci)
	//std::vector<MemoryChunk> emissionLight;
	//std::vector<MemoryChunk> reachedLight;
	//std::vector<MemoryChunk> verticies;			// Przekszta³cone wierzcho³ki i normalne.

	thrust::device_vector<glm::vec3>		deviceEmissionLight;			// Œwiat³o, które dany trójk¹t mo¿e wyemitowaæ.
	thrust::device_vector<glm::vec3>		deviceReachedLight;				// Kolor danego trójk¹ta, wynikaj¹cy z poch³aniania œwiat³a dochodzacego.
	thrust::device_vector<VertexFormat>		deviceVerticies;				// Wierzcho³ki meshy przekszta³cone do uk³adu wspó³rzêdnych œwiata.

	std::vector<Size> chunkOffsets;

	Prepare( deviceEmissionLight, deviceReachedLight, deviceVerticies, chunkOffsets );
	Radiosity( deviceEmissionLight, deviceReachedLight, deviceVerticies, chunkOffsets );
	BuildResult( deviceReachedLight );

	m_lightmapState = LightmapState::GenerationEnded;		// Ustaw na koñcu funkcji, ¿eby w¹tek g³ówny móg³ zebraæ wyniki.
}


class TransformVerticies : thrust::unary_function < VertexFormat, VertexNormalTexCord1 >
{
private:
	glm::mat4 modelWorldTransform;
public:
	TransformVerticies( glm::mat4 matrix )
		: modelWorldTransform( matrix ) {}

	VertexFormat operator()( VertexNormalTexCord1& vertex )
	{
		glm::vec4 position = glm::vec4( glm::make_vec3( &vertex.position ), 1.0f );
		position = modelWorldTransform * position;
		VertexFormat result;
		result.position = glm::vec3( position );
	}
};



/**@brief Przygotowuje odpowiednie struktury do generacji.*/
void LightmapWorkerCUDA::Prepare( thrust::device_vector<glm::vec3>& emissionLight, thrust::device_vector<glm::vec3>& reachedLight, thrust::device_vector<VertexFormat>& verticies, std::vector<Size>& chunkOffsets )
{
	auto& parts = m_data->objectParts;
	Size bufferLength = 0;
	std::vector<glm::vec3> materialDiffuse;

	// Budujemy tablicê offsetów w buforze dla ka¿dej poczêœci mesha.
	// Dla ka¿dej czêœci przepisujemy te¿ materia³.
	// Zoptymalizowaæ w póŸniejszych wersjach. Materia³ mo¿e siê powtarzaæ dla ró¿nych podmeshy.
	for( auto& part : parts )
	{
		Size chunkSize = part.verticesCount / 3;
		
		chunkOffsets.push_back( bufferLength );
		materialDiffuse.push_back( glm::vec3( part.diffuse.x, part.diffuse.y, part.diffuse.z ) );
		bufferLength += chunkSize;

		assert( materialDiffuse.size() <= MAX_MATERIALS );
	}

	// Wype³niamy bufory zerami. Dla œwiate³ wektor emissionLight zostanie wype³niony póŸniej odpowiednimi wartoœciami.
	emissionLight.resize( bufferLength, glm::vec3( 0.0, 0.0, 0.0 ) );
	reachedLight.resize( bufferLength, glm::vec3( 0.0, 0.0, 0.0 ) );
	verticies.resize( bufferLength * 3 );
	
	// Pêtla po wszystkich podmeshach.
	// @todo zoptymalizowaæ to w przysz³ej wersji. Mo¿e da siê to zrobiæ jakoœ m¹drzej za jednym razem.
	for( unsigned int i = 0; i < parts.size(); ++i )
	{
		// @note Macierze DirectXMath s¹ row-major a glm column-major. Trzeba trnasponowaæ.
		glm::mat4 transformMatrix = glm::make_mat4( (void*)&parts[i].transform );
		glm::transpose( transformMatrix );

		thrust::device_vector<VertexNormalTexCord1> sourceVerticies = GetVerticiesFromGraphicAPI( m_data->buffers[ m_data->objectParts[ i ].chunkIdx ] );
		
		// Przekszta³ca wierzcho³ki do uk³adu wspó³rzêdnych œwiata.
		thrust::transform(	sourceVerticies.begin() + m_data->objectParts[ i ].bufferOffset,
							sourceVerticies.begin() + m_data->objectParts[ i ].bufferOffset + m_data->objectParts[ i ].verticesCount,
							verticies.begin() + chunkOffsets[ i ] * 3,
							TransformVerticies( transformMatrix ) );


		// Sprawdzamy czy dany podobiekt mesha jest œwiat³em - tzn. zawiera niezerowe wartoœci emisji materia³u.
		if( parts[ i ].emissive.x != 0.0f || parts[ i ].emissive.y != 0.0f || parts[ i ].emissive.z != 0.0f )
		{
			glm::vec3 materialEmissive( parts[i].emissive.x, parts[i].emissive.y, parts[i].emissive.z );
			// Wype³niamy wektor emisji dla danego kawa³ka mesha.
			thrust::fill( emissionLight.begin() + chunkOffsets[ i ], emissionLight.begin() + chunkOffsets[ i + 1 ], materialEmissive );
		}


		UnmapVerticiesFromGraphicAPI();
	}

	// Wype³niamy pamiêæ sta³¹
	cudaMemcpyToSymbol( materials, materialDiffuse.data(), materialDiffuse.size() );
}

/**@brief Wykonuje algorytm radiosity.*/
void LightmapWorkerCUDA::Radiosity( thrust::device_vector<glm::vec3>& emissionLight,
									thrust::device_vector<glm::vec3>& reachedLight,
									thrust::device_vector<VertexFormat>& verticies,
									std::vector<Size>& chunkOffsets )
{
	unsigned emissionLightSize = emissionLight.size();
	unsigned verticiesSize = verticies.size();
	unsigned reachedLightSize = reachedLight.size();

	// Bufor g³êbokoœci i bufor s³u¿¹cy do indeksowania trójk¹tów, które znajduj¹ siê w buforze.
	depthBufferSize = m_depthResolution * m_depthResolution;

	float*				deviceDepthBuffer;//( depthSize * sizeof( float ) );
	BufferIndexing*		deviceIndexBuffer;//( depthSize * sizeof( BufferIndexing ) );

	checkCudaErrors( cudaMalloc( (void**)&deviceDepthBuffer, depthBufferSize * sizeof( float ) ) );
	checkCudaErrors( cudaMalloc( (void**)&deviceIndexBuffer, depthBufferSize * sizeof( BufferIndexing ) ) );

	// Identyfikuje wielok¹t z najwiêksz¹ energi¹ do wyemitowania.
	std::tuple<unsigned int, unsigned int, float> emissionMax = FindMaxEmission( emissionLight, emissionLight.size() );

	// Koñczymy generowanie, gdy najwiêksza zgromadzona w wielok¹cie energia spadnie poni¿ej pewnego poziomu.
	int iterations = 0;
	while( iterations < 30 )
	{
		iterations++;

		// Czyœci bufory 
		KernelClearDepthIndex << < ( depthBufferSize + 255 ) / 256, 256 >> > ( deviceDepthBuffer, deviceIndexBuffer );
		checkCudaErrors( cudaGetLastError() );

		// Wykonuje przebieg budowania bufora Z
		DepthPass( emissionMax, verticies, deviceDepthBuffer, deviceIndexBuffer, chunkOffsets );

		// Wykonuje transfer energii pomiêdzy p³atami
		TransferPass( emissionMax, verticies, emissionLight, reachedLight, deviceIndexBuffer, chunkOffsets );

		emissionMax = FindMaxEmission( emissionLight, emissionLight.size() );
	}

	cudaFree( deviceDepthBuffer );
	cudaFree( deviceIndexBuffer );
}

/**W tej funkcji trzeba zebraæ z wektora reachedLight dane o kolorze i przepisaæ
do buforów razem ze wspó³rzêdnymi uv.
Dodatkowo tu bêdzie siê odbywa³o interpolowanie wartoœci w celu wyg³adzenia obrazu.
*/
void LightmapWorkerCUDA::BuildResult( thrust::device_vector<glm::vec3>& reachedLight )
{
	CoordColor*		outputBuffer;	// Pary (wspó³rzêdna uv, kolor).

	// 

	//auto& parts = m_data->objectParts;
	//auto object = parts[ 0 ].object;
	//unsigned int verticiesCount = 0;
	//unsigned int firstPartIndex = 0;

	//for( unsigned int i = 0; i < parts.size(); ++i )
	//{
	//	verticiesCount += parts[ i ].verticesCount;

	//	// Sumujemy liczbê wierzcho³ków w obiekcie. Dopiero jak zacznie siê nowy obiekt to tworzymy bufor.
	//	unsigned int nextIdx = i + 1;
	//	if( nextIdx == parts.size() || parts[ nextIdx ].object != object )	// Pierwszy warunek wymusza, ¿e weŸmiemy tak¿e ostatni obiekt
	//	{
	//		if( verticiesCount != 0 )
	//		{
	//			MemoryChunk colorMap( verticiesCount * sizeof( CoordColor ) );

	//			unsigned int verticiesOffset = 0;
	//			for( unsigned int j = firstPartIndex; j < nextIdx; ++j )
	//			{
	//				for( unsigned int k = 0; k < reachedLight[ j ].Count<glm::vec3>(); ++k )
	//				{
	//					CoordColor& colorVertex1 = colorMap.Get<CoordColor>( verticiesOffset + mul3( k ) );		// Jest 3 razy wiêcej wierzcho³ków ni¿ kolorów.
	//					glm::vec3& lightColor = reachedLight[ j ].Get<glm::vec3>( k ); /*glm::vec3( 0.5, 0.3, 0.7);*/
	//					colorVertex1.color = lightColor;
	//					colorVertex1.texCoords = m_data->verticies[ parts[ j ].chunkIdx ].Get<VertexNormalTexCord1>( parts[ j ].bufferOffset + mul3( k ) ).tex_cords;

	//					CoordColor& colorVertex2 = colorMap.Get<CoordColor>( verticiesOffset + mul3( k ) + 1 );
	//					colorVertex2.color = lightColor;
	//					colorVertex2.texCoords = m_data->verticies[ parts[ j ].chunkIdx ].Get<VertexNormalTexCord1>( parts[ j ].bufferOffset + mul3( k ) + 1 ).tex_cords;

	//					CoordColor& colorVertex3 = colorMap.Get<CoordColor>( verticiesOffset + mul3( k ) + 2 );
	//					colorVertex3.color = lightColor;
	//					colorVertex3.texCoords = m_data->verticies[ parts[ j ].chunkIdx ].Get<VertexNormalTexCord1>( parts[ j ].bufferOffset + mul3( k ) + 2 ).tex_cords;
	//				}

	//				verticiesOffset += parts[ j ].verticesCount;
	//			}

	//			m_resultData.push_back( std::move( colorMap ) );
	//		}

	//		if( nextIdx == parts.size() )		break;		// Nie ma wiêcej obiektów.
	//		// Zakoñczyliœmy przetwarzanie obiektu przechodzimy do nastêpnego.
	//		object = parts[ nextIdx ].object;
	//		firstPartIndex = nextIdx;
	//		verticiesCount = 0;
	//	}

	//}
}


/**@brief Wykonuje przebieg z z-buforem.*/
void LightmapWorkerCUDA::DepthPass( std::tuple<unsigned int, unsigned int, float>& emissionMax,
									thrust::device_vector<VertexFormat>& verticies,
									float* deviceDepthBuffer,
									BufferIndexing* deviceIndexBuffer,
									std::vector<Size>& chunkOffsets )
{
	// Pobieramy indeksy emitera.
	unsigned int idx1 = std::get<0>( emissionMax );
	unsigned int idx2 = std::get<1>( emissionMax );

	// Wyliczamy macierz widoku, która przekszta³ca wierzcho³ki do uk³adu wspó³rzêdnych zwi¹zanego z emiterem.
	// Wersja na procesorze jest tylko tymczasowa.
	Triangle4 emiterPosition( &( verticies.data() + chunkOffsets[ idx1 ] * 3 ).get()[ idx2 ] );
	glm::mat4 emiterViewMatrix = EmiterViewMatrix( emiterPosition );

	// Pêtla po wszystkich podobiektach. Rozwi¹zanie tymczasowe do póŸniejszego poprawienia przy optymalizacjach.
	for( unsigned int i = 0; chunkOffsets.size() - 1; ++i )
	{
		KernelDepthPass << < ( chunkOffsets[ i + 1 ] - chunkOffsets[ i ] + 255 ) / 256, 256 >> > ( verticies.data().get() + chunkOffsets[ i ], chunkOffsets[ i + 1 ] - chunkOffsets[ i ], i, deviceDepthBuffer, deviceIndexBuffer, emiterPosition, emiterViewMatrix );
		checkCudaErrors( cudaGetLastError() );
	}
}

void LightmapWorkerCUDA::TransferPass( std::tuple<unsigned int, unsigned int, float>& emissionMax,
									   thrust::device_vector<VertexFormat>& verticies,
									   thrust::device_vector<glm::vec3>& emissionLight,
									   thrust::device_vector<glm::vec3>& reachedLight,
									   BufferIndexing* indexBuffer,
									   std::vector<Size>& chunkOffsets )
{
	// Pobieramy indeks emitera i zerujemy maksymaln¹ emisjê. Bêdziemy szukaæ nowej
	// maksymalnej wartoœci w tym przebiegu.
	std::get<2>( emissionMax ) = 0.0f;
	unsigned int idx1 = std::get<0>( emissionMax );
	unsigned int idx2 = std::get<1>( emissionMax );

	// Wyliczamy ile energii powinien dostaæ ka¿dy element, który znajdziemy w buforze indeksów.
	// Zoptymalizowaæ: wszystko powinno siê odbywaæ na karcie bez powrotów.
	glm::vec3 depthElementWeight = glm::vec3( static_cast<float>( 50.0 * 4.0 * XM_1DIV2PI / depthBufferSize ) );
	glm::vec3 emitedLight = ( emissionLight.data().get() + chunkOffsets[ idx1 ] )[ idx2 ];
	emitedLight = emitedLight * depthElementWeight;

	// Zerujemy œwiat³o, które emiter bêdzie móg³ wyemitowaæ w kolejnych iteracjach.
	( emissionLight.data().get() + chunkOffsets[ idx1 ] )[ idx2 ] = glm::vec3( 0.0, 0.0, 0.0 );

	KernelTransferPass << < ( depthBufferSize + 255 ) / 256, 256 >> > ( indexBuffer, reachedLight.data().get(), emissionLight.data().get(), emitedLight );

	checkCudaErrors( cudaGetLastError() );
}



