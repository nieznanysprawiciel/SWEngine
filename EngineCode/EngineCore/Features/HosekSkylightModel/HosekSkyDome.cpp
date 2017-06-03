#include "EngineCore/stdafx.h"
#include "HosekSkyDome.h"

using namespace DirectX;

#include "Common/MemoryLeaks.h"
#include "EngineCore/ModelsManager/DefaultAssets.h"

#include "EngineCore/ModelsManager/Assets/Meshes/MeshAssetInitData.h"



HosekSkyDome::HosekSkyDome( )
	:	SkyDome()
	,	m_backIdxBuffer( nullptr )
	,	m_backVertexBuffer( nullptr )
{}


HosekSkyDome::~HosekSkyDome()
{
	if ( m_backIdxBuffer )
		delete[] m_backIdxBuffer;
	if ( m_backVertexBuffer )
		delete[] m_backVertexBuffer;
}

/**@brief Inicjuje kopu�� nieba i wylicza kolory dla poszczeg�lnych wierzcho�k�w.

W tej funkcji jest tworzony mesh kopu�y, nast�pnie tworzone s� bufory indeks�w i wierzcho�k�w
oraz layout struktury wierzcho�ka. Kompilowane i dodawane s� r�wnie� shadery.

Zalecam u�ywanie nieaprzystej liczby wierzcho�k�w w pionie. Wtedy utworzy si� r�wnik
na kopule nieba, dzi�ki czemu niebo b�dzie lepiej wygl�da�o. Warto�ci kolor�w obliczane s�
dla wierzcho�k�w, a pomi�dzy nimi s� interpolowane przez rasterizer. Poni�ej r�wnika
brany jest jeden z kolor�w wygenerowanych powy�ej i ustawiany dla ca�ej kopu�y.

@note Raz utworzony mesh kopu�y ju� nigdy si� nie zmienia. Ta funkcja powinna zosta�
wywo�ana tylko raz na pocz�tku, potem nale�y wywo�ywa� funkcj� UpdateSkyDome.

@attention Ta funkcja musi zosta� wykonana przed dodaniem obiektu do DisplayEngine.

@param[in] sun_direction Wektor w kierunku s�o�ca. Zostanie znormalizowany w funkcji.
@param[in] turbidity M�tno�� atmosfery.
@param[in] albedo Tablica 3-elementowa warto�ci albedo dla poszczeg�lnych kana��w RGB.
@param[in] vertical_vert Liczba segment�w kopu�y nieba w pionie.
@param[in] horizontal_vert Liczba segment�w kopu�y nieba w poziomie.
@param[in] radius Promie� kuli kopu�y nieba.
@param[in] sky_intensity Mno�nik jasno�ci nieba, je�eli nie pda si� nic, warto�� domy�lna wynosi 1.0.
@param[in] sun_intensity Mno�nik jasno�ci tarczy s�onecznej. Domy�lnie jest ustawiane na 1.0.
*/
void HosekSkyDome::InitSkyDome( XMVECTOR sun_direction,
								  double turbidity,
								  const double* albedo,
								  int vertical_vert,
								  int horizontal_vert,
								  float radius,
								  float sky_intensity,
								  float sun_intensity )
{
	// Generujemy kopu��
	GenerateSphere( vertical_vert, horizontal_vert, radius );

	// Zak�adamy, �e tablice jeszcze nie istania�y. Obliczamy rozmiary tablic ( w liczbie element�w )
	int vert_buff_elements = (vertical_vert - 2) * horizontal_vert + 2;		// W pionie b�dzie vertical-2 pas�w wierzcho�k�w + musimy dopisa� dwa wierzcho�ki skrajne
	int ind_buff_elements = 2 * 3 * (vertical_vert - 2) * horizontal_vert;	// Liczba pas�w czworok�t�w, razy dwa (�eby zrobi� tr�j�ty) razy 3, �eby przeliczy� na liczb� wierzcho�k�w

	//set_index_buffer( L"HosekWilkieModel_index_buffer", m_backIdxBuffer, sizeof(VERT_INDEX), ind_buff_elements );
	//set_vertex_buffer( L"HosekWilkieModel_vertex_buffer", m_backVertexBuffer, sizeof(SkyDomeVertex), vert_buff_elements );
	//display_data.mesh->use_index_buf = true;
	//display_data.mesh->buffer_offset = 0;
	//display_data.mesh->base_vertex = 0;
	//display_data.mesh->vertices_count = ind_buff_elements;

	//// Kompilujemy shadery, jednocze�nie dodaje si� layout
	//set_vertex_shader( L"shaders\\HosekSkyDome\\HosekSkyDome.fx", DefaultAssets::DEFAULT_VERTEX_SHADER_ENTRY, DefaultAssets::LAYOUT_POSITION_COLOR );
	//set_pixel_shader( L"shaders\\HosekSkyDome\\HosekSkyDome.fx", DefaultAssets::DEFAULT_PIXEL_SHADER_ENTRY );

	//// Materia� jest niepotrzebny, ale nie mo�e go nie by�
	//MaterialObject material;
	//material.SetNullMaterial();
	//set_material( &material, DefaultAssets::DEFAULT_MATERIAL_STRING );

	//// Tutaj wype�niamy kopu�� kolorem
	//UpdateSkyDome( sun_direction, turbidity, albedo, sky_intensity, sun_intensity );


	auto engine = GetEngineInterface();

	ShaderInputLayout* layout;

	MaterialInitData materialInfo;
	materialInfo.VertexShader = engine->Assets.Shaders.LoadVertexShaderSync( L"shaders\\HosekSkyDome\\HosekSkyDome.fx", &layout, DefaultAssets::LAYOUT_POSITION_COLOR );
	materialInfo.PixelShader = engine->Assets.Shaders.LoadPixelShaderSync( L"shaders\\HosekSkyDome\\HosekSkyDome.fx" );

	MeshCreateData meshInfo;
	meshInfo.VertexBuffer = engine->Assets.Buffers.CreateVertexBufferSync( L"::HosekWilkieModel_VertexBuffer", (uint8*)m_backVertexBuffer, vert_buff_elements, sizeof( SkyDomeVertex ) );
	meshInfo.IndexBuffer = engine->Assets.Buffers.CreateVertexBufferSync( L"::HosekWilkieModel_IndexBuffer", (uint8*)m_backIdxBuffer, ind_buff_elements, sizeof( VERT_INDEX ) );
	meshInfo.VertexLayout = layout;

	meshInfo.MeshSegments.resize( 1 );
	meshInfo.MeshSegments[ 0 ].Material = engine->Assets.Materials.CreateMaterialSync( L"::HosekWilkieModel_Material", std::move( materialInfo ) );
	meshInfo.MeshSegments[ 0 ].BaseVertex = 0;
	meshInfo.MeshSegments[ 0 ].BufferOffset = 0;
	meshInfo.MeshSegments[ 0 ].NumVertices = ind_buff_elements;
	meshInfo.MeshSegments[ 0 ].Topology = PrimitiveTopology::PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	SetModel( engine->Assets.Meshes.CreateMeshSync( L"::HosekWilkieModel_Mesh", std::move( meshInfo ) ) );

	UpdateSkyDome( sun_direction, turbidity, albedo, sky_intensity, sun_intensity );
}

/**@brief Funkcja ta s�u�y do przeliczenia na nowo danych o kolorach.

Kolory s� przeliczane i zapisywane do bufora tylnego, a nast�pnie po zako�czeniu generowania
uaktualniany jest bufor wierzcho�k�w i indeks�w. Funkcja jest jest przygotowana do wykonywania
w innym watku, ale nie powinno si� wykonywa� wielu instancji tej funkcji jednocze�nie, bo nie jest to 
bezpieczne.

@note Funkcja nie modyfikuje �adnych danych klasy SkyDome, jedynie aktualizuje bufor kolor�w
nowymi warto�ciami. W szczeg�lno�ci nie jest modyfimkowana struktura wierzcho�k�w.

@param[in] sun_direction Wektor w kierunku s�o�ca.
@param[in] turbidity M�tno�� atmosfery.
@param[in] albedo Tablica 3-elementowa warto�ci albedo dla poszczeg�lnych kana��w RGB.
@param[in] sky_intensity Mno�nik jasno�ci nieba, je�eli nie pda si� nic, warto�� domy�lna wynosi 1.0.
@param[in] sun_intensity Mno�nik jasno�ci tarczy s�onecznej. Domy�lnie jest ustawiane na 1.0.*/
void HosekSkyDome::UpdateSkyDome( XMVECTOR sun_direction,
									double turbidity,
									const double* albedo,
									float sky_intensity,
									float sun_intensity)
{
	// Obliczamy wysoko�� s�o�ca
	XMVECTOR zenith = XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f );
	sun_direction = XMVector3Normalize( sun_direction );
	XMVECTOR solar_elevation = XMVector3AngleBetweenNormals( sun_direction, zenith );
	float elevation = DirectX::XM_PIDIV2 - XMVectorGetX( solar_elevation );

	m_skylightModel.init( turbidity, albedo, elevation, sky_intensity, sun_intensity );

	for ( unsigned int i = 0; i < m_verticiesCount; ++i )
	{
		// Iterujemy po wszyskich wierzcho�kach w buforze
		XMVECTOR view_direction = XMLoadFloat3( &m_backVertexBuffer[i].position );
		view_direction = XMVector3Normalize( view_direction );
		XMVECTOR angle = XMVector3AngleBetweenNormals( zenith, view_direction );
		float theta = XMVectorGetX( angle );
		
		if ( theta > XM_PIDIV2 )
		{	// Je�eli jestesmy poni�ej horyzontu to na razie malujemy na czerwono
			m_backVertexBuffer[i].color = m_backVertexBuffer[m_verticiesCount/2 - 2].color;
			continue;
		}

		angle = XMVector3AngleBetweenNormals( sun_direction, view_direction );
		float gamma = XMVectorGetX( angle );
		// Pobieramy kolor dla danego wierzcho�ka
		XMVECTOR color = m_skylightModel.sky_radiance( theta, gamma );
		// Zapisujemy kolor
		XMStoreFloat3( &m_backVertexBuffer[i].color, color );

		//m_backVertexBuffer[i].color = XMFLOAT3( 1.0f, 0.0f, 0.0f );	// Test
	}

	RequestUpdate();		// DisplayEngine zaktualizuje bufor
}

/**@brief Funkcja generuje w m_backVertexBuffer i m_backIdxBuffer sfer� dla kpu�y nieba.
Jest to sfera z punktami wyr�nionymi na g�rze i dole. (nie jest to geosfera)

@param[in] vertical Liczba wierzcho�k�w w pionie.
@param[in] horizontal Liczba wierzcho�k�w w poziomie.
@param[in] radius Promie� kopu�y nieba.*/
void HosekSkyDome::GenerateSphere( int vertical, int horizontal, float radius )
{
	// Zak�adamy, �e tablice jeszcze nie istania�y. Obliczamy ile potrzeba zaalokowa�
	int vert_buff_elements = (vertical - 2) * horizontal + 2;	// W pionie b�dzie vertical-2 pas�w wierzcho�k�w + musimy dopisa� dwa wierzcho�ki skrajne
	int ind_buff_elements = 2 * 3 * (vertical - 2) * horizontal;	// Liczba pas�w czworok�t�w, razy dwa (�eby zrobi� tr�j�ty) razy 3, �eby przeliczy� na liczb� wierzcho�k�w
	m_verticiesCount = vert_buff_elements;		// Zapisujemy warto�� na sta��
	
	m_backVertexBuffer = new SkyDomeVertex[vert_buff_elements];
	m_backIdxBuffer = new VERT_INDEX[ind_buff_elements];

	float vert_angle = DirectX::XM_PI / (float)horizontal;	// Obliczamy przesuni�cie k�towe w pionie
	float hor_angle = DirectX::XM_2PI / (float)vertical;	// Obliczamy przesuni�cie k�towe w poziomie


	// Zaczynamy od wierzcho�ka g�rnego i idziemy w d�
	m_backVertexBuffer[0] = { XMFLOAT3( 0.0, radius, 0.0 ), XMFLOAT3( 0.0, 0.0, 0.0 ) };

	unsigned int cur_ptr = 1;
	for ( int i = 1; i < vertical - 1; ++i )
	{
		float beta = vert_angle * i;		// K�t w p�aszczy�nie poziomej
		float r = radius * sin( beta );		// Promie� zrzutowany na p�aszczyzn� poziom�
		float y = radius * cos( beta );		// Wsp�rz�dna y wektora

		for ( int j = 0; j < horizontal; ++j )
		{
			float alfa = hor_angle * j;		// K�t w p�aszczy�nie pionowej

			float x = r * cos( alfa );
			float z = r * sin( alfa );

			m_backVertexBuffer[cur_ptr++] = { XMFLOAT3(x, y, z), XMFLOAT3( 0.0, 0.0, 0.0 ) };
		}
	}
	// Dolny wierzcho�ek
	m_backVertexBuffer[vert_buff_elements - 1] = { XMFLOAT3( 0.0, -radius, 0.0 ), XMFLOAT3( 0.0, 0.0, 0.0 ) };

	// Mamy ju� wszystkie wierzcho�ki, teraz musimy je po��czy�. Wype�niamy bufor indeks�w.
	cur_ptr = 0;		// Wskazuje na miejsce w buforze indeks�w do wype�nienia
	int i = 1;			// Wskazuje na wierzcho�ek z ni�szej warstwy
	for ( ; i < horizontal; ++i )	// Odliczamy horizontal wierzcho�k�w od nr 1
	{
		// Na razie wype�niamy tylko g�rn� wartw� z punktem centralnym
		m_backIdxBuffer[cur_ptr++] = 0;
		m_backIdxBuffer[cur_ptr++] = i;
		m_backIdxBuffer[cur_ptr++] = i + 1 ;
	}
	// Niestety trzeba rozwa�y� osobno
	m_backIdxBuffer[cur_ptr++] = 0;
	m_backIdxBuffer[cur_ptr++] = horizontal;
	m_backIdxBuffer[cur_ptr++] = 1;

	for ( int k = 1; k < vertical - 2; ++k )
	{
		for ( int h = 0; h < horizontal - 1; ++h )
		{
			int j = (k - 1) * horizontal + h + 1;	// Wskazuje na wierzcho�ek z wy�szej warstwy
			i = j + horizontal;						// Wskazuje na wierzcho�ek z ni�szej warstwy

			// Wype�niamy po dwa tr�jk�ty
			m_backIdxBuffer[cur_ptr++] = j;
			m_backIdxBuffer[cur_ptr++] = i;
			m_backIdxBuffer[cur_ptr++] = i + 1;

			m_backIdxBuffer[cur_ptr++] = j;
			m_backIdxBuffer[cur_ptr++] = i + 1;
			m_backIdxBuffer[cur_ptr++] = j + 1;
		}
		// Ostatnie wierzcho�ki z ka�dego pasa musimy rozwa�y� osobno
		int j = k  * horizontal;	// Wskazuje na wierzcho�ek z wy�szej warstwy

		// To jest miejsce, w kt�rym ostatni wierzcho�ek z pasa ��czy si� z pierwszym, kt�ry zosta� ju� rozwa�ony.
		// Wierzcho�ki s� roz�o�one w pami�ci ci�g�ej, wi�c za ostatnim wierzcho�kiem z g�rnego rz�du jest pierwszy z dolnego.
		m_backIdxBuffer[cur_ptr++] = j;
		m_backIdxBuffer[cur_ptr++] = j + horizontal;
		m_backIdxBuffer[cur_ptr++] = j + 1;

		m_backIdxBuffer[cur_ptr++] = j;
		m_backIdxBuffer[cur_ptr++] = j + 1;
		m_backIdxBuffer[cur_ptr++] = (k-1)*horizontal + 1;
	}

	// Teraz musimy pod��czy� ten pojedynczy wierzcho�ek na dole, do wszystkich z ostatniego pasa
	// Dla odmiany idziemy od ty�u
	for ( int k = vert_buff_elements - 2; k > vert_buff_elements - horizontal - 2; --k )
	{
		m_backIdxBuffer[cur_ptr++] = vert_buff_elements - 1;
		m_backIdxBuffer[cur_ptr++] = k;
		m_backIdxBuffer[cur_ptr++] = k - 1;
	}
}

/**@brief Funkcja aktualizuje bufor wierzcho�k�w zawarto�ci� tylnego bufora.

Aby zaktualizowa� niebo trzeba wywo�a� funkcje UpdateSkyDome (w jakim� oddzielnym w�tku).
Funkcja ustawi zmienn� update_vertex_buffer po zako�czeniu aktualizacji tylnego bufora.
Nast�pnie DisplayEngine odczyta zmienn� update_vertex_buffer i wywo�a funkcj� UpdateBuffers.

Funkcja wirtualna, przeczytaj opis funkcji SkyDome::UpdateBuffers.*/
void HosekSkyDome::UpdateBuffers( IRenderer* renderer )
{
	auto mesh = GetModel();

	renderer->UpdateSubresource( mesh->GetVertexBufferRawPtr(), m_backVertexBuffer );

	Updated();
}
