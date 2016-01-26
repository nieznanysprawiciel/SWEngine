#include "EngineEditor/LightmapsTool/stdafx.h"

// To makro musi by� zdefiniowane przed zaincludowaniem DirectXMath (w LightmapWorkerCUDA.h).
// Biblioteka u�ywa domy�lnie zestawu instrukcji SSE2 i trzeba to wy��czy�.
#define _XM_NO_INTRINSICS_

#include "EngineEditor/LightmapsTool/LightmapWorkers/LightmapWorkerCUDA.h"
#include "EngineCore/ModelsManager/DefaultAssets.h"

#include <cstring>
#include <limits>
// Trzeba oddefiniowa�, je�eli chce si� korzysta� z numeric_limits
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
// Funkcje pomocnicze - og�lne
// ============================================================================= //


/**@brief Pobiera dane o wierzcho�kach zgromadzone na karcie graficznej.

@todo Funkcja zostanie zaimplementowana w przysz�o�ci.

Dane o wierzcho�kach meshy s� zgromadzone ju� na karcie graficznej.
Ta funkcja ma umo�liwi� pobranie wska�nik�w na te dane z obiektu BufferObject, kt�ry
S�u�y opakowaniu obiekt�w DirectXowych (lub w przysz�o�ci tak�e innych API graficznych).

Na razie dla sp�jno�ci kodu jest zwracany obiekt thrust::device_vector<VertexNormalTexCord1>.
W przysz�o�ci b�dzie to prawdopodobnie go�y wska�nik na pami�� i reszt� aplikacje b�dzie trzeba dostosowa�.

Do pobierania wska�nik�w pos�u�� funkcje:
cudaGraphicsMapResources
cudaGraphicsResourceGetMappedPointer*/
thrust::device_vector<VertexNormalTexCord1> GetVerticiesFromGraphicAPI( BufferObject* buffer )
{
	return thrust::device_vector<VertexNormalTexCord1>();
}

/**@brief Zmapowane wierzcho�ki funkcj� GetVerticiesFromGraphicAPI trzeba "odmapowa�" po zako�czeniu przetwarzania.*/
void UnmapVerticiesFromGraphicAPI()
{

}


/**@brief Szybsze (miejmy nadziej�) mno�enie przez 3.*/
inline unsigned int mul3( unsigned int number )
{
	return ( number << 1 ) + number;
}

inline float fmax( float a, float b, float c )
{
	return fmax( a, fmax( b, c ) );
}

/**@brief Maksymalna warto�� trzech liczb.*/
int max3( float x1, float x2, float x3 )
{
	if( x1 > x2 )
		return x1 > x3 ? (int)x1 : (int)x3;
	else
		return x2 > x3 ? (int)x2 : (int)x3;
}

/**@brief Minimalna warto�� trzech liczb.*/
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

/**@brief Funkcja znajduje trojk�t o najwi�kszej zgromadznej energii do wyemitowania.

Prawdopodobnie b�dzie to robione jak�� form� redukcji.*/
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

/**@brief Wylicza wsp�rz�dn� barycentryczn� punktu wzgl�dem kraw�dzie.
Wsp�rz�dne nie s� wyskalowane do jedynki. Nale�y je wyskalowa� kiedy si� wyliczy wszystkie.*/
__device__ static float BarycentricCoords( glm::vec2& vertex1, glm::vec2& vertex2, glm::ivec2& point )
{
	return ( vertex2.x - vertex1.x ) * ( point.y - vertex1.y ) - ( vertex2.y - vertex1.y ) * ( point.x - vertex1.x );
}

/*
@param[inout] receiver Tr�jk�t do zrzutowania.W tej zmiennej zwracany jest tr�jk�t wynikowy.
@return Zwraca wektor z g��boko�ciami dla ka�dego tr�jk�ta.Je�eli jaki� wierzcho�ek
znajduje si� za p�aszczyzn� rzutowania, funkcja zwraca(-1.0, -1.0, -1.0).W normalnej sytuacji
wszystkie wsp�rz�dne s� dodatnie. */
__device__ static glm::vec3 HemisphereCast( Triangle4& emiter, Triangle4& receiver, glm::mat4& emiterViewMatrix )
{
	// Transformujemy wierzcho�ki do uk�adu emitera. Dzi�ki temu xy jest pozycj� na p�aszczy�nie (na razie jeszcze nie jednostkowej)
	// a z jest odleg�o�ci� od emitera.
	// @todo Zastanowi� sie czy nie da si� tu mno�y� przez macierz 3x3.
	glm::vec4 centerToVertex1 = emiterViewMatrix * glm::vec4( receiver.vertex1, 1.0f );		// Poniewa� to s� wierzcho�ki, to ustawiamy komponent w na 1.
	glm::vec4 centerToVertex2 = emiterViewMatrix * glm::vec4( receiver.vertex2, 1.0f );
	glm::vec4 centerToVertex3 = emiterViewMatrix * glm::vec4( receiver.vertex3, 1.0f );

	// Wyliczenie d�ugo�ci od �rodka do wierzcho�k�w.
	float depth1 = glm::length( glm::vec3( centerToVertex1 ) );
	float depth2 = glm::length( glm::vec3( centerToVertex2 ) );
	float depth3 = glm::length( glm::vec3( centerToVertex3 ) );

	// Normalizacja wektor�w
	receiver.vertex1 = glm::vec3( centerToVertex1 / depth1 );
	receiver.vertex2 = glm::vec3( centerToVertex2 / depth2 );
	receiver.vertex3 = glm::vec3( centerToVertex3 / depth3 );

	// Po tym przekszta�ceniu wektor jest wyra�ony we wsp�rz�dnych p�aszczyzny. Sk�adowa z oznacza
	// odleg�o�� obiektu od p�aszczyzny. Uwaga! Nie jest to odleg�o�� od �rodka p�aszczyzny, ale
	// odleg�o�� rzutu prostopad�ego na t� p�aszczyzn�. Mimo wszystko taka g��boko�� pozwala na poprawne
	// wyznaczenie przys�aniania.
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

/**@brief Rasteryzuje podany tr�jk�t. Sprawdza czy jest bli�ej emitera od ostatnio
wpisanego tr�jk�ta na podstawie bufora g��boko�ci i je�eli tak, to zapisuje nowy tr�jk�t.

Rasteryzacja zrobiona mniej wi�cej na podstawie strony:
https://fgiesen.wordpress.com/2013/02/06/the-barycentric-conspirac/
Warto przeczyta� te� kilka temat�w pokrewnych. Na razie implementuj� wersj� niewydajn�.
Mo�e potem przerobi� na lepsz�. W ka�dym razie warto zauwa�y�, �e ten algorytm jest bardzo dobry
do zr�wnoleglania, mo�e da si� to jako� sprytnie wykorzysta�.

@param[in] triangle Tr�jk�t do rasteryzacji.
@param[in] depths Odleg�o�ci (g��boko�ci) kolejnych wierzcho�k�w tr�jk�ta od emitera. (Tu jest ma�e oszustwo,
zobaczymy jak b�dzie si� sprawdza�, odleg�o�ci s� liczone do �rodka emitera).
@param[in] chunkIdx Indeks MemoryChunka do wpisania do bufora indeks�w.
@param[in] triangleIdx Indeks tr�jk�ta, kt�ry trafi do bufora indeks�w.
@param[in] depthBuffer Bufor g��boko�ci.
@param[in] indexBuffer Bufor indeks�w.*/
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

	// Wyliczamy prostok�t ograniczaj�cy.
	int minX = min3( triangles[ 0 ].x, triangles[ 1 ].x, triangles[ 2 ].x );
	int minY = min3( triangles[ 0 ].y, triangles[ 1 ].y, triangles[ 2 ].y );
	int maxX = max3( triangles[ 0 ].x, triangles[ 1 ].x, triangles[ 2 ].x );
	int maxY = max3( triangles[ 0 ].y, triangles[ 1 ].y, triangles[ 2 ].y );

	// Iterujemy po wszystkich elementach prostok�ta ograniczaj�cego.
	// Sprawdzamy czy dany punkt nale�y do tr�jk�ta. Je�eli nale�y
	// zagl�damy do z bufora.
	glm::ivec2 point;
	for( point.y = minY; point.y <= maxY; point.y++ )
	{
		for( point.x = minX; point.x <= maxX; point.x++ )
		{
			float w0 = BarycentricCoords( triangles[ 1 ], triangles[ 2 ], point );
			float w1 = BarycentricCoords( triangles[ 2 ], triangles[ 0 ], point );
			float w2 = BarycentricCoords( triangles[ 0 ], triangles[ 1 ], point );

			if( w0 >= 0 && w1 >= 0 && w2 >= 0 )	// Warunek na to, �e punkt jest wewn�trz tr�jk�ta.
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

/**@brief Transformuje wierzcho�ki w przedziale [-1,1] do wsp�rzednych bufora g��boko�ci.*/
__device__ static void HemisphereViewport( Triangle4& receiver, unsigned int m_depthResolution )
{
	glm::vec3 depthResolution = glm::vec3( (float)m_depthResolution / 2.0f );
	
	receiver.vertex1 = receiver.vertex1 * depthResolution + depthResolution;
	receiver.vertex2 = receiver.vertex2 * depthResolution + depthResolution;
	receiver.vertex3 = receiver.vertex3 * depthResolution + depthResolution;
}

/**@brief Tworzy macierz przekszta�caj�c� wierzcho�ki do uk�adu odniesienia emitera.*/
__host__ glm::mat4 LightmapWorkerCUDA::EmiterViewMatrix( Triangle4& emiter )
{
	// �rodek ci�ko�ci tr�jk�ta przyjujemy za �rodek emitera.
	glm::vec3 emiterCenter = emiter.vertex1 + emiter.vertex2;
	emiterCenter = emiterCenter + emiter.vertex3;
	emiterCenter = emiterCenter + glm::vec3( 3.0f );

	// Wektor prostopad�y do p�aszczyzny, na kt�rej le�� tr�jk�ty.
	glm::vec3 edge12 = emiter.vertex2 - emiter.vertex1;
	glm::vec3 edge13 = emiter.vertex3 - emiter.vertex1;
	glm::vec3 normal = glm::normalize( glm::cross( edge12, edge13 ) );

	// Ustalamy dowolnie (byle ortogonalnie) kierunek "w g�r�".
	glm::vec3 upDirection = glm::normalize( glm::cross( edge12, normal ) );

	// Tworzymy macierz widoku.
	glm::mat4 viewMatrix = glm::lookAt( emiterCenter, normal, upDirection );

	return viewMatrix;
}

// ============================================================================= //
// Generation - kernele
// ============================================================================= //

// @todo Zast�pi� lepszym rozwi�zaniem ni� zmienna globalna.
unsigned int depthBufferSize = 0;

/**@brief Czy�ci bufory g��boko�ci i indeks�w.*/
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
	// Pierwsza wsp�rz�dna wierzcho�ka
	int j = blockIdx.x * blockDim.x + threadIdx.x;
	if( j > vertexMax ) return;


	// Wyliczamy pozycj� odbiorcy w uk�adzie wsp�rz�dnych bufora g��boko�ci.
	Triangle4 receiverPosition( verticies + j );
	glm::vec3 receiverDepths = HemisphereCast( emiterPosition, receiverPosition, emiterViewMatrix );

	if( receiverDepths.x < 0.0f || receiverDepths.y < 0.0f || receiverDepths.z < 0.0f )
		return;  // Tr�jk�t jest cz�ciowo za p�aszczyzn�. Musimy go odrzuci�.

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
	// Tylko piksele na kole powinny dosta� dawk� energii, a bufor jest kwadratem.
	// Dlatego elementy, na kt�re nie zosta� zrzutowany �aden tr�jk�t zosta�y zainicjowane warto�ci� INVALID_INDEX.
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

/**@brief G��wna funkcja generuj�ca lightmapy. Zaimplementuj w klasie pochodnej.*/
void LightmapWorkerCUDA::Generate()
{
	if( m_data->objectParts.empty() )
		return;

	checkCudaErrors( cudaSetDevice( 0 ) );


	//// Chunki dla ca�ych obiek�w (nie dla podcz�ci)
	//std::vector<MemoryChunk> emissionLight;
	//std::vector<MemoryChunk> reachedLight;
	//std::vector<MemoryChunk> verticies;			// Przekszta�cone wierzcho�ki i normalne.

	thrust::device_vector<glm::vec3>		deviceEmissionLight;			// �wiat�o, kt�re dany tr�jk�t mo�e wyemitowa�.
	thrust::device_vector<glm::vec3>		deviceReachedLight;				// Kolor danego tr�jk�ta, wynikaj�cy z poch�aniania �wiat�a dochodzacego.
	thrust::device_vector<VertexFormat>		deviceVerticies;				// Wierzcho�ki meshy przekszta�cone do uk�adu wsp�rz�dnych �wiata.

	std::vector<Size> chunkOffsets;

	Prepare( deviceEmissionLight, deviceReachedLight, deviceVerticies, chunkOffsets );
	Radiosity( deviceEmissionLight, deviceReachedLight, deviceVerticies, chunkOffsets );
	BuildResult( deviceReachedLight );

	m_lightmapState = LightmapState::GenerationEnded;		// Ustaw na ko�cu funkcji, �eby w�tek g��wny m�g� zebra� wyniki.
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

	// Budujemy tablic� offset�w w buforze dla ka�dej pocz�ci mesha.
	// Dla ka�dej cz�ci przepisujemy te� materia�.
	// Zoptymalizowa� w p�niejszych wersjach. Materia� mo�e si� powtarza� dla r�nych podmeshy.
	for( auto& part : parts )
	{
		Size chunkSize = part.verticesCount / 3;
		
		chunkOffsets.push_back( bufferLength );
		materialDiffuse.push_back( glm::vec3( part.diffuse.x, part.diffuse.y, part.diffuse.z ) );
		bufferLength += chunkSize;

		assert( materialDiffuse.size() <= MAX_MATERIALS );
	}

	// Wype�niamy bufory zerami. Dla �wiate� wektor emissionLight zostanie wype�niony p�niej odpowiednimi warto�ciami.
	emissionLight.resize( bufferLength, glm::vec3( 0.0, 0.0, 0.0 ) );
	reachedLight.resize( bufferLength, glm::vec3( 0.0, 0.0, 0.0 ) );
	verticies.resize( bufferLength * 3 );
	
	// P�tla po wszystkich podmeshach.
	// @todo zoptymalizowa� to w przysz�ej wersji. Mo�e da si� to zrobi� jako� m�drzej za jednym razem.
	for( unsigned int i = 0; i < parts.size(); ++i )
	{
		// @note Macierze DirectXMath s� row-major a glm column-major. Trzeba trnasponowa�.
		glm::mat4 transformMatrix = glm::make_mat4( (void*)&parts[i].transform );
		glm::transpose( transformMatrix );

		thrust::device_vector<VertexNormalTexCord1> sourceVerticies = GetVerticiesFromGraphicAPI( m_data->buffers[ m_data->objectParts[ i ].chunkIdx ] );
		
		// Przekszta�ca wierzcho�ki do uk�adu wsp�rz�dnych �wiata.
		thrust::transform(	sourceVerticies.begin() + m_data->objectParts[ i ].bufferOffset,
							sourceVerticies.begin() + m_data->objectParts[ i ].bufferOffset + m_data->objectParts[ i ].verticesCount,
							verticies.begin() + chunkOffsets[ i ] * 3,
							TransformVerticies( transformMatrix ) );


		// Sprawdzamy czy dany podobiekt mesha jest �wiat�em - tzn. zawiera niezerowe warto�ci emisji materia�u.
		if( parts[ i ].emissive.x != 0.0f || parts[ i ].emissive.y != 0.0f || parts[ i ].emissive.z != 0.0f )
		{
			glm::vec3 materialEmissive( parts[i].emissive.x, parts[i].emissive.y, parts[i].emissive.z );
			// Wype�niamy wektor emisji dla danego kawa�ka mesha.
			thrust::fill( emissionLight.begin() + chunkOffsets[ i ], emissionLight.begin() + chunkOffsets[ i + 1 ], materialEmissive );
		}


		UnmapVerticiesFromGraphicAPI();
	}

	// Wype�niamy pami�� sta��
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

	// Bufor g��boko�ci i bufor s�u��cy do indeksowania tr�jk�t�w, kt�re znajduj� si� w buforze.
	depthBufferSize = m_depthResolution * m_depthResolution;

	float*				deviceDepthBuffer;//( depthSize * sizeof( float ) );
	BufferIndexing*		deviceIndexBuffer;//( depthSize * sizeof( BufferIndexing ) );

	checkCudaErrors( cudaMalloc( (void**)&deviceDepthBuffer, depthBufferSize * sizeof( float ) ) );
	checkCudaErrors( cudaMalloc( (void**)&deviceIndexBuffer, depthBufferSize * sizeof( BufferIndexing ) ) );

	// Identyfikuje wielok�t z najwi�ksz� energi� do wyemitowania.
	std::tuple<unsigned int, unsigned int, float> emissionMax = FindMaxEmission( emissionLight, emissionLight.size() );

	// Ko�czymy generowanie, gdy najwi�ksza zgromadzona w wielok�cie energia spadnie poni�ej pewnego poziomu.
	int iterations = 0;
	while( iterations < 30 )
	{
		iterations++;

		// Czy�ci bufory 
		KernelClearDepthIndex << < ( depthBufferSize + 255 ) / 256, 256 >> > ( deviceDepthBuffer, deviceIndexBuffer );
		checkCudaErrors( cudaGetLastError() );

		// Wykonuje przebieg budowania bufora Z
		DepthPass( emissionMax, verticies, deviceDepthBuffer, deviceIndexBuffer, chunkOffsets );

		// Wykonuje transfer energii pomi�dzy p�atami
		TransferPass( emissionMax, verticies, emissionLight, reachedLight, deviceIndexBuffer, chunkOffsets );

		emissionMax = FindMaxEmission( emissionLight, emissionLight.size() );
	}

	cudaFree( deviceDepthBuffer );
	cudaFree( deviceIndexBuffer );
}

/**W tej funkcji trzeba zebra� z wektora reachedLight dane o kolorze i przepisa�
do bufor�w razem ze wsp�rz�dnymi uv.
Dodatkowo tu b�dzie si� odbywa�o interpolowanie warto�ci w celu wyg�adzenia obrazu.
*/
void LightmapWorkerCUDA::BuildResult( thrust::device_vector<glm::vec3>& reachedLight )
{
	CoordColor*		outputBuffer;	// Pary (wsp�rz�dna uv, kolor).

	// 

	//auto& parts = m_data->objectParts;
	//auto object = parts[ 0 ].object;
	//unsigned int verticiesCount = 0;
	//unsigned int firstPartIndex = 0;

	//for( unsigned int i = 0; i < parts.size(); ++i )
	//{
	//	verticiesCount += parts[ i ].verticesCount;

	//	// Sumujemy liczb� wierzcho�k�w w obiekcie. Dopiero jak zacznie si� nowy obiekt to tworzymy bufor.
	//	unsigned int nextIdx = i + 1;
	//	if( nextIdx == parts.size() || parts[ nextIdx ].object != object )	// Pierwszy warunek wymusza, �e we�miemy tak�e ostatni obiekt
	//	{
	//		if( verticiesCount != 0 )
	//		{
	//			MemoryChunk colorMap( verticiesCount * sizeof( CoordColor ) );

	//			unsigned int verticiesOffset = 0;
	//			for( unsigned int j = firstPartIndex; j < nextIdx; ++j )
	//			{
	//				for( unsigned int k = 0; k < reachedLight[ j ].Count<glm::vec3>(); ++k )
	//				{
	//					CoordColor& colorVertex1 = colorMap.Get<CoordColor>( verticiesOffset + mul3( k ) );		// Jest 3 razy wi�cej wierzcho�k�w ni� kolor�w.
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

	//		if( nextIdx == parts.size() )		break;		// Nie ma wi�cej obiekt�w.
	//		// Zako�czyli�my przetwarzanie obiektu przechodzimy do nast�pnego.
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

	// Wyliczamy macierz widoku, kt�ra przekszta�ca wierzcho�ki do uk�adu wsp�rz�dnych zwi�zanego z emiterem.
	// Wersja na procesorze jest tylko tymczasowa.
	Triangle4 emiterPosition( &( verticies.data() + chunkOffsets[ idx1 ] * 3 ).get()[ idx2 ] );
	glm::mat4 emiterViewMatrix = EmiterViewMatrix( emiterPosition );

	// P�tla po wszystkich podobiektach. Rozwi�zanie tymczasowe do p�niejszego poprawienia przy optymalizacjach.
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
	// Pobieramy indeks emitera i zerujemy maksymaln� emisj�. B�dziemy szuka� nowej
	// maksymalnej warto�ci w tym przebiegu.
	std::get<2>( emissionMax ) = 0.0f;
	unsigned int idx1 = std::get<0>( emissionMax );
	unsigned int idx2 = std::get<1>( emissionMax );

	// Wyliczamy ile energii powinien dosta� ka�dy element, kt�ry znajdziemy w buforze indeks�w.
	// Zoptymalizowa�: wszystko powinno si� odbywa� na karcie bez powrot�w.
	glm::vec3 depthElementWeight = glm::vec3( static_cast<float>( 50.0 * 4.0 * XM_1DIV2PI / depthBufferSize ) );
	glm::vec3 emitedLight = ( emissionLight.data().get() + chunkOffsets[ idx1 ] )[ idx2 ];
	emitedLight = emitedLight * depthElementWeight;

	// Zerujemy �wiat�o, kt�re emiter b�dzie m�g� wyemitowa� w kolejnych iteracjach.
	( emissionLight.data().get() + chunkOffsets[ idx1 ] )[ idx2 ] = glm::vec3( 0.0, 0.0, 0.0 );

	KernelTransferPass << < ( depthBufferSize + 255 ) / 256, 256 >> > ( indexBuffer, reachedLight.data().get(), emissionLight.data().get(), emitedLight );

	checkCudaErrors( cudaGetLastError() );
}



