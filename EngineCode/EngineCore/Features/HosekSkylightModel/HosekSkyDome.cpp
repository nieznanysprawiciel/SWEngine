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

/**@brief Inicjuje kopu³ê nieba i wylicza kolory dla poszczególnych wierzcho³ków.

W tej funkcji jest tworzony mesh kopu³y, nastêpnie tworzone s¹ bufory indeksów i wierzcho³ków
oraz layout struktury wierzcho³ka. Kompilowane i dodawane s¹ równie¿ shadery.

Zalecam u¿ywanie nieaprzystej liczby wierzcho³ków w pionie. Wtedy utworzy siê równik
na kopule nieba, dziêki czemu niebo bêdzie lepiej wygl¹da³o. Wartoœci kolorów obliczane s¹
dla wierzcho³ków, a pomiêdzy nimi s¹ interpolowane przez rasterizer. Poni¿ej równika
brany jest jeden z kolorów wygenerowanych powy¿ej i ustawiany dla ca³ej kopu³y.

@note Raz utworzony mesh kopu³y ju¿ nigdy siê nie zmienia. Ta funkcja powinna zostaæ
wywo³ana tylko raz na pocz¹tku, potem nale¿y wywo³ywaæ funkcjê UpdateSkyDome.

@attention Ta funkcja musi zostaæ wykonana przed dodaniem obiektu do DisplayEngine.

@param[in] sun_direction Wektor w kierunku s³oñca. Zostanie znormalizowany w funkcji.
@param[in] turbidity Mêtnoœæ atmosfery.
@param[in] albedo Tablica 3-elementowa wartoœci albedo dla poszczególnych kana³ów RGB.
@param[in] vertical_vert Liczba segmentów kopu³y nieba w pionie.
@param[in] horizontal_vert Liczba segmentów kopu³y nieba w poziomie.
@param[in] radius Promieñ kuli kopu³y nieba.
@param[in] sky_intensity Mno¿nik jasnoœci nieba, je¿eli nie pda siê nic, wartoœæ domyœlna wynosi 1.0.
@param[in] sun_intensity Mno¿nik jasnoœci tarczy s³onecznej. Domyœlnie jest ustawiane na 1.0.
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
	// Generujemy kopu³ê
	GenerateSphere( vertical_vert, horizontal_vert, radius );

	// Zak³adamy, ¿e tablice jeszcze nie istania³y. Obliczamy rozmiary tablic ( w liczbie elementów )
	int vert_buff_elements = (vertical_vert - 2) * horizontal_vert + 2;		// W pionie bêdzie vertical-2 pasów wierzcho³ków + musimy dopisaæ dwa wierzcho³ki skrajne
	int ind_buff_elements = 2 * 3 * (vertical_vert - 2) * horizontal_vert;	// Liczba pasów czworok¹tów, razy dwa (¿eby zrobiæ trój¹ty) razy 3, ¿eby przeliczyæ na liczbê wierzcho³ków

	//set_index_buffer( L"HosekWilkieModel_index_buffer", m_backIdxBuffer, sizeof(VERT_INDEX), ind_buff_elements );
	//set_vertex_buffer( L"HosekWilkieModel_vertex_buffer", m_backVertexBuffer, sizeof(SkyDomeVertex), vert_buff_elements );
	//display_data.mesh->use_index_buf = true;
	//display_data.mesh->buffer_offset = 0;
	//display_data.mesh->base_vertex = 0;
	//display_data.mesh->vertices_count = ind_buff_elements;

	//// Kompilujemy shadery, jednoczeœnie dodaje siê layout
	//set_vertex_shader( L"shaders\\HosekSkyDome\\HosekSkyDome.fx", DefaultAssets::DEFAULT_VERTEX_SHADER_ENTRY, DefaultAssets::LAYOUT_POSITION_COLOR );
	//set_pixel_shader( L"shaders\\HosekSkyDome\\HosekSkyDome.fx", DefaultAssets::DEFAULT_PIXEL_SHADER_ENTRY );

	//// Materia³ jest niepotrzebny, ale nie mo¿e go nie byæ
	//MaterialObject material;
	//material.SetNullMaterial();
	//set_material( &material, DefaultAssets::DEFAULT_MATERIAL_STRING );

	//// Tutaj wype³niamy kopu³ê kolorem
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

/**@brief Funkcja ta s³u¿y do przeliczenia na nowo danych o kolorach.

Kolory s¹ przeliczane i zapisywane do bufora tylnego, a nastêpnie po zakoñczeniu generowania
uaktualniany jest bufor wierzcho³ków i indeksów. Funkcja jest jest przygotowana do wykonywania
w innym watku, ale nie powinno siê wykonywaæ wielu instancji tej funkcji jednoczeœnie, bo nie jest to 
bezpieczne.

@note Funkcja nie modyfikuje ¿adnych danych klasy SkyDome, jedynie aktualizuje bufor kolorów
nowymi wartoœciami. W szczególnoœci nie jest modyfimkowana struktura wierzcho³ków.

@param[in] sun_direction Wektor w kierunku s³oñca.
@param[in] turbidity Mêtnoœæ atmosfery.
@param[in] albedo Tablica 3-elementowa wartoœci albedo dla poszczególnych kana³ów RGB.
@param[in] sky_intensity Mno¿nik jasnoœci nieba, je¿eli nie pda siê nic, wartoœæ domyœlna wynosi 1.0.
@param[in] sun_intensity Mno¿nik jasnoœci tarczy s³onecznej. Domyœlnie jest ustawiane na 1.0.*/
void HosekSkyDome::UpdateSkyDome( XMVECTOR sun_direction,
									double turbidity,
									const double* albedo,
									float sky_intensity,
									float sun_intensity)
{
	// Obliczamy wysokoœæ s³oñca
	XMVECTOR zenith = XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f );
	sun_direction = XMVector3Normalize( sun_direction );
	XMVECTOR solar_elevation = XMVector3AngleBetweenNormals( sun_direction, zenith );
	float elevation = DirectX::XM_PIDIV2 - XMVectorGetX( solar_elevation );

	m_skylightModel.init( turbidity, albedo, elevation, sky_intensity, sun_intensity );

	for ( unsigned int i = 0; i < m_verticiesCount; ++i )
	{
		// Iterujemy po wszyskich wierzcho³kach w buforze
		XMVECTOR view_direction = XMLoadFloat3( &m_backVertexBuffer[i].position );
		view_direction = XMVector3Normalize( view_direction );
		XMVECTOR angle = XMVector3AngleBetweenNormals( zenith, view_direction );
		float theta = XMVectorGetX( angle );
		
		if ( theta > XM_PIDIV2 )
		{	// Je¿eli jestesmy poni¿ej horyzontu to na razie malujemy na czerwono
			m_backVertexBuffer[i].color = m_backVertexBuffer[m_verticiesCount/2 - 2].color;
			continue;
		}

		angle = XMVector3AngleBetweenNormals( sun_direction, view_direction );
		float gamma = XMVectorGetX( angle );
		// Pobieramy kolor dla danego wierzcho³ka
		XMVECTOR color = m_skylightModel.sky_radiance( theta, gamma );
		// Zapisujemy kolor
		XMStoreFloat3( &m_backVertexBuffer[i].color, color );

		//m_backVertexBuffer[i].color = XMFLOAT3( 1.0f, 0.0f, 0.0f );	// Test
	}

	RequestUpdate();		// DisplayEngine zaktualizuje bufor
}

/**@brief Funkcja generuje w m_backVertexBuffer i m_backIdxBuffer sferê dla kpu³y nieba.
Jest to sfera z punktami wyró¿nionymi na górze i dole. (nie jest to geosfera)

@param[in] vertical Liczba wierzcho³ków w pionie.
@param[in] horizontal Liczba wierzcho³ków w poziomie.
@param[in] radius Promieñ kopu³y nieba.*/
void HosekSkyDome::GenerateSphere( int vertical, int horizontal, float radius )
{
	// Zak³adamy, ¿e tablice jeszcze nie istania³y. Obliczamy ile potrzeba zaalokowaæ
	int vert_buff_elements = (vertical - 2) * horizontal + 2;	// W pionie bêdzie vertical-2 pasów wierzcho³ków + musimy dopisaæ dwa wierzcho³ki skrajne
	int ind_buff_elements = 2 * 3 * (vertical - 2) * horizontal;	// Liczba pasów czworok¹tów, razy dwa (¿eby zrobiæ trój¹ty) razy 3, ¿eby przeliczyæ na liczbê wierzcho³ków
	m_verticiesCount = vert_buff_elements;		// Zapisujemy wartoœæ na sta³ê
	
	m_backVertexBuffer = new SkyDomeVertex[vert_buff_elements];
	m_backIdxBuffer = new VERT_INDEX[ind_buff_elements];

	float vert_angle = DirectX::XM_PI / (float)horizontal;	// Obliczamy przesuniêcie k¹towe w pionie
	float hor_angle = DirectX::XM_2PI / (float)vertical;	// Obliczamy przesuniêcie k¹towe w poziomie


	// Zaczynamy od wierzcho³ka górnego i idziemy w dó³
	m_backVertexBuffer[0] = { XMFLOAT3( 0.0, radius, 0.0 ), XMFLOAT3( 0.0, 0.0, 0.0 ) };

	unsigned int cur_ptr = 1;
	for ( int i = 1; i < vertical - 1; ++i )
	{
		float beta = vert_angle * i;		// K¹t w p³aszczyŸnie poziomej
		float r = radius * sin( beta );		// Promieñ zrzutowany na p³aszczyznê poziom¹
		float y = radius * cos( beta );		// Wspó³rzêdna y wektora

		for ( int j = 0; j < horizontal; ++j )
		{
			float alfa = hor_angle * j;		// K¹t w p³aszczyŸnie pionowej

			float x = r * cos( alfa );
			float z = r * sin( alfa );

			m_backVertexBuffer[cur_ptr++] = { XMFLOAT3(x, y, z), XMFLOAT3( 0.0, 0.0, 0.0 ) };
		}
	}
	// Dolny wierzcho³ek
	m_backVertexBuffer[vert_buff_elements - 1] = { XMFLOAT3( 0.0, -radius, 0.0 ), XMFLOAT3( 0.0, 0.0, 0.0 ) };

	// Mamy ju¿ wszystkie wierzcho³ki, teraz musimy je po³¹czyæ. Wype³niamy bufor indeksów.
	cur_ptr = 0;		// Wskazuje na miejsce w buforze indeksów do wype³nienia
	int i = 1;			// Wskazuje na wierzcho³ek z ni¿szej warstwy
	for ( ; i < horizontal; ++i )	// Odliczamy horizontal wierzcho³ków od nr 1
	{
		// Na razie wype³niamy tylko górn¹ wartwê z punktem centralnym
		m_backIdxBuffer[cur_ptr++] = 0;
		m_backIdxBuffer[cur_ptr++] = i;
		m_backIdxBuffer[cur_ptr++] = i + 1 ;
	}
	// Niestety trzeba rozwa¿yæ osobno
	m_backIdxBuffer[cur_ptr++] = 0;
	m_backIdxBuffer[cur_ptr++] = horizontal;
	m_backIdxBuffer[cur_ptr++] = 1;

	for ( int k = 1; k < vertical - 2; ++k )
	{
		for ( int h = 0; h < horizontal - 1; ++h )
		{
			int j = (k - 1) * horizontal + h + 1;	// Wskazuje na wierzcho³ek z wy¿szej warstwy
			i = j + horizontal;						// Wskazuje na wierzcho³ek z ni¿szej warstwy

			// Wype³niamy po dwa trójk¹ty
			m_backIdxBuffer[cur_ptr++] = j;
			m_backIdxBuffer[cur_ptr++] = i;
			m_backIdxBuffer[cur_ptr++] = i + 1;

			m_backIdxBuffer[cur_ptr++] = j;
			m_backIdxBuffer[cur_ptr++] = i + 1;
			m_backIdxBuffer[cur_ptr++] = j + 1;
		}
		// Ostatnie wierzcho³ki z ka¿dego pasa musimy rozwa¿yæ osobno
		int j = k  * horizontal;	// Wskazuje na wierzcho³ek z wy¿szej warstwy

		// To jest miejsce, w którym ostatni wierzcho³ek z pasa ³¹czy siê z pierwszym, który zosta³ ju¿ rozwa¿ony.
		// Wierzcho³ki s¹ roz³o¿one w pamiêci ci¹g³ej, wiêc za ostatnim wierzcho³kiem z górnego rzêdu jest pierwszy z dolnego.
		m_backIdxBuffer[cur_ptr++] = j;
		m_backIdxBuffer[cur_ptr++] = j + horizontal;
		m_backIdxBuffer[cur_ptr++] = j + 1;

		m_backIdxBuffer[cur_ptr++] = j;
		m_backIdxBuffer[cur_ptr++] = j + 1;
		m_backIdxBuffer[cur_ptr++] = (k-1)*horizontal + 1;
	}

	// Teraz musimy pod³¹czyæ ten pojedynczy wierzcho³ek na dole, do wszystkich z ostatniego pasa
	// Dla odmiany idziemy od ty³u
	for ( int k = vert_buff_elements - 2; k > vert_buff_elements - horizontal - 2; --k )
	{
		m_backIdxBuffer[cur_ptr++] = vert_buff_elements - 1;
		m_backIdxBuffer[cur_ptr++] = k;
		m_backIdxBuffer[cur_ptr++] = k - 1;
	}
}

/**@brief Funkcja aktualizuje bufor wierzcho³ków zawartoœci¹ tylnego bufora.

Aby zaktualizowaæ niebo trzeba wywo³aæ funkcje UpdateSkyDome (w jakimœ oddzielnym w¹tku).
Funkcja ustawi zmienn¹ update_vertex_buffer po zakoñczeniu aktualizacji tylnego bufora.
Nastêpnie DisplayEngine odczyta zmienn¹ update_vertex_buffer i wywo³a funkcjê UpdateBuffers.

Funkcja wirtualna, przeczytaj opis funkcji SkyDome::UpdateBuffers.*/
void HosekSkyDome::UpdateBuffers( IRenderer* renderer )
{
	auto mesh = GetModel();

	renderer->UpdateSubresource( mesh->GetVertexBufferRawPtr(), m_backVertexBuffer );

	Updated();
}
