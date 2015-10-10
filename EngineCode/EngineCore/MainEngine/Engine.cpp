/**@file Engine.cpp
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.

@brief Plik zawiera definicje metod klasy Engine dotycz¹cych inicjacji i zwalniania DirectXa
oraz g³ówne funkcje do renderingu.
*/


#include "EngineCore/stdafx.h"
#include "Engine.h"
#include "GraphicAPI/ResourcesFactory.h"
#include "EngineCore/EngineHelpers/PerformanceCheck.h"
#include "EngineCore/EventsManager/Event.h"
#include "EngineCore/CollisionEngine/CollisionEngine.h"
#include "EngineCore/ControllersEngine/ControllersEngine.h"
#include "EngineCore/GamePlay/FableEngine.h"
#include "EngineCore/DisplayEngine/DisplayEngine.h"
#include "EngineCore/ModelsManager/ModelsManager.h"
#include "EngineCore/PhysicEngine/PhysicEngine.h"
#include "EngineCore/PhysicEngine/MovementEngine.h"
#include "EngineCore/SoundEngine/SoundEngine.h"
#include "EngineCore/UIEngine/UI_Engine.h"
#include "EngineCore/GamePlay/IGamePlay.h"

#include "Common/memory_leaks.h"




USE_PERFORMANCE_CHECK( RENDERING_TIME )
USE_PERFORMANCE_CHECK( FRAME_COMPUTING_TIME )
#ifdef _INTERPOLATE_POSITIONS
	USE_PERFORMANCE_CHECK( INTERPOLATION_TIME )
#endif


//----------------------------------------------------------------------------------------------//
//								konstruktor i destruktor										//
//----------------------------------------------------------------------------------------------//


Engine::Engine(HINSTANCE instance)
{
	REGISTER_PERFORMANCE_TASK(RENDERING_TIME)
	REGISTER_PERFORMANCE_TASK(FRAME_COMPUTING_TIME)
#ifdef _INTERPOLATE_POSITIONS
	REGISTER_PERFORMANCE_TASK(INTERPOLATION_TIME)
#endif

	// Dziêki tej zmiennej bêdzie mo¿na wysy³aæ eventy
	Object::set_engine( this );

#ifndef __UNUSED
	//Zmienna decyduje o konczeniu w¹tków
	join_render_thread = false;
#endif

	events_queue = nullptr;

	full_screen = false;			//inicjalizacja jako false potrzebna w funkcji init_window
	m_engineReady = false;			//jeszcze nie zainicjowaliœmy
	instance_handler = instance;

	// Initialize global strings
	LoadString(instance_handler, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(instance_handler, IDC_SW_ENGINE, szWindowClass, MAX_LOADSTRING);

	m_graphicInitializer = ResourcesFactory::CreateAPIInitializer();

	controllers_engine	=	new ControllersEngine(this);
	movement_engine		=	new MovementEngine(this);
	display_engine		=	new DisplayEngine(this);
	collision_engine	=	new CollisionEngine(this);
	physic_engine		=	new PhysicEngine(this);
	models_manager		=	new ModelsManager(this);
	fable_engine		=	new FableEngine(this);
	sound_engine		=	new SoundEngine(this);
	ui_engine			=	new UI_Engine(this);

	//inicjujemy licznik klatek
	pause = false;
}


Engine::~Engine()
{

#ifdef __TEST
	//obiekty trzeba pokasowaæ, zanim siê skasuje to, do czego siê odwo³uj¹
	for ( unsigned int i = 0; i < object_list.size(); ++i )
		delete object_list[i];
#endif

	delete controllers_engine;
	delete movement_engine;
	delete display_engine;
	delete collision_engine;
	delete physic_engine;
	delete fable_engine;
	delete sound_engine;
	delete ui_engine;			//sprz¹ta po directinpucie
	delete models_manager;		//musi byæ kasowany na koñcu

	DefaultAssets::Release();
	m_graphicInitializer->ReleaseAPI();
	delete m_graphicInitializer;
}


//----------------------------------------------------------------------------------------------//
//								inicjalizacja okna i modu³ów zewnêtrznych						//
//----------------------------------------------------------------------------------------------//

///@brief Inicjuje dzia³anie silnika.
///W trybie fullscreen szerokoœæ i wysokoœæ okna jest ignorowana, a dane s¹ pobierane z systemu.
///@param[in] width Szerokoœæ okna
///@param[in] height Wysokoœæ okna
///@param[in] fullscreen Pe³ny ekran lub renderowanie w oknie
///@param[in] nCmdShow Czwarty parametr funkcji WinMain. Decyduje w jaki sposób powinno zostaæ pokazane okno aplikacji.
int Engine::InitEngine( int width, int height, bool fullScreen, int nCmdShow )
{
	int result;

	//Tworzenie okna aplikacji
	result = InitWindow( width, height, fullScreen, nCmdShow );
	if ( !result )
		return FALSE;

	//Inicjalizowanie API graficznego
	result = InitGraphicAPI( width, height, fullScreen );
	assert( result != 0 );
	if( result == 0 )
		return FALSE;

	result = InitInputModule();
	assert( result != 0 );
	if( result == 0 )
		return FALSE;

	result = InitSoundModule();
	assert( result != 0 );
	if( result == 0 )
		return FALSE;

	result = InitDefaultAssets();
	assert( result != 0 );
	if( result == 0 )
		return FALSE;

	result = InitDisplayer();
	assert( result != 0 );
	if( result == 0 )
		return FALSE;

	// Czym póŸniej zainicjujemy tym lepiej.
	m_timeManager.InitTimer();

	m_engineReady = true;		//jesteœmy gotowi do renderowania

	return TRUE;
}



bool Engine::InitGraphicAPI( int width, int height, bool full_screen )
{
	bool result;

	//Inicjalizowanie API graficznego
	GraphicAPIInitData initData;
	initData.fullScreen			= full_screen;
	initData.singleThreaded		= false;
	initData.windowHandle		= (uint32)window_handler;
	initData.windowHeight		= height;
	initData.windowWidth		= width;
	initData.depthStencilFormat = ResourceFormat::RESOURCE_FORMAT_D24_UNORM_S8_UINT;
	result = m_graphicInitializer->InitAPI( initData );
	assert( result != 0 );
	if( result == 0 )
		return false;
	return true;
}

bool Engine::InitInputModule		()
{
	int result;

	//Inicjalizowanie directXinputa
	result = ui_engine->init_direct_input( );
		assert( result == DIRECT_INPUT_OK );	//Dzia³a tylko w trybie DEBUG
	if ( result != DIRECT_INPUT_OK )
		return false;
	return true;
}

bool Engine::InitSoundModule		()
{
	return true;
}

//----------------------------------------------------------------------------------------------//
//								Przygotowanie modu³ów do dzia³ania								//
//----------------------------------------------------------------------------------------------//

/**@brief Inicjuje domyœlne assety silnika.*/
bool Engine::InitDefaultAssets()
{
	DefaultAssets::Init();

	ShaderInputLayoutObject* layout;
	models_manager->AddVertexShader( DEFAULT_VERTEX_SHADER_STRING, DEFAULT_VERTEX_SHADER_ENTRY, &layout, DefaultAssets::LAYOUT_POSITION_NORMAL_COORD );
	models_manager->AddPixelShader( DEFAULT_PIXEL_SHADER_STRING, DEFAULT_PIXEL_SHADER_ENTRY );
	models_manager->AddPixelShader( DEFAULT_TEX_DIFFUSE_PIXEL_SHADER_PATH, DEFAULT_PIXEL_SHADER_ENTRY );

	display_engine->SetLayout( layout );		///@todo Hack. Layout powinien byæ ustawialny dla ka¿dego mesha z osobna. Zlikwidowaæ.

	MaterialObject* nullMaterial = new MaterialObject();
	nullMaterial->SetNullMaterial();
	models_manager->AddMaterial( nullMaterial, DEFAULT_MATERIAL_STRING );

	RenderTargetObject* mainRenderTarget = ResourcesFactory::CreateScreenRenderTarget();
	models_manager->AddRenderTarget( mainRenderTarget, SCREEN_RENDERTARGET_STRING );

	return true;
}

/**@brief Inicjalizuje DisplayEngine.

Funkcja tworzy renderery, domyœlne bufory sta³ych oraz ustawia macierz projekcji.
@return Zwraca zawsze true.*/
bool Engine::InitDisplayer()
{
	IRenderer* renderer = m_graphicInitializer->CreateRenderer( RendererUsage::USE_AS_IMMEDIATE );
	display_engine->InitRenderer( renderer );
	display_engine->InitDisplayer( models_manager );

	display_engine->SetProjectionMatrix( XMConvertToRadians( 45 ),
										   (float)window_width / (float)window_height, 1, 100000 );

	return true;
}



//----------------------------------------------------------------------------------------------//
//								potok przetwarzania obiektów									//
//----------------------------------------------------------------------------------------------//

/**@brief To tutaj dziej¹ siê wszystkie rzeczy, które s¹ wywo³ywane co ka¿d¹ klatkê.*/
void Engine::RenderFrame()
{
	float time_interval = m_timeManager.onStartRenderFrame();
	float lag = m_timeManager.GetFrameLag();


	while ( lag >= FIXED_MOVE_UPDATE_INTERVAL )
	{
		START_PERFORMANCE_CHECK(FRAME_COMPUTING_TIME)

		ui_engine->proceed_input( FIXED_MOVE_UPDATE_INTERVAL );
		physic_engine->proceed_physic( FIXED_MOVE_UPDATE_INTERVAL );
		controllers_engine->proceed_controllers_pre( FIXED_MOVE_UPDATE_INTERVAL );
		movement_engine->proceed_movement( FIXED_MOVE_UPDATE_INTERVAL );
		controllers_engine->proceed_controllers_post( FIXED_MOVE_UPDATE_INTERVAL );
		collision_engine->proceed_collisions( FIXED_MOVE_UPDATE_INTERVAL );
		fable_engine->proceed_fable( FIXED_MOVE_UPDATE_INTERVAL );

		lag -= FIXED_MOVE_UPDATE_INTERVAL;
		m_timeManager.UpdateTimeLag( lag );

		END_PERFORMANCE_CHECK( FRAME_COMPUTING_TIME )
	}


#ifdef _INTERPOLATE_POSITIONS
	START_PERFORMANCE_CHECK( INTERPOLATION_TIME )

	display_engine->interpolate_positions( lag / FIXED_MOVE_UPDATE_INTERVAL );

	END_PERFORMANCE_CHECK( INTERPOLATION_TIME )
#endif

	START_PERFORMANCE_CHECK( RENDERING_TIME )

	//Renderujemy scenê oraz interfejs u¿ytkownika
	display_engine->BeginScene();

	display_engine->display_scene( time_interval, lag / FIXED_MOVE_UPDATE_INTERVAL );
	ui_engine->draw_GUI( time_interval, lag / FIXED_MOVE_UPDATE_INTERVAL );

	END_PERFORMANCE_CHECK( RENDERING_TIME )		///< Ze wzglêdu na V-sync test wykonujemy przed wywyo³aniem funkcji present.

	display_engine->EndScene();
}



/**
@brief Aktualizuje po³o¿enia obiektów.

Wywo³ywane s¹ funkcje odpowiadaj¹ce za fizykê, ruch, kontrolery, wejœcie od u¿ytkownika i fabu³ê.

@param[inout] lag Przeliczanie po³o¿eñ obiektów odbywa siê ze sta³ym interwa³em czasowym.
Zmienna okreœla, ile up³yne³o czasu od ostatniego przeliczenia po³o¿eñ.
@param[in] timeInterval Czas jaki up³yn¹³ od ostatniej klatki.
*/
void Engine::UpdateScene( float& lag, float timeInterval )
{
	while ( lag >= FIXED_MOVE_UPDATE_INTERVAL )
	{
		START_PERFORMANCE_CHECK(FRAME_COMPUTING_TIME)

		ui_engine->proceed_input( FIXED_MOVE_UPDATE_INTERVAL );
		physic_engine->proceed_physic( FIXED_MOVE_UPDATE_INTERVAL );
		controllers_engine->proceed_controllers_pre( FIXED_MOVE_UPDATE_INTERVAL );
		movement_engine->proceed_movement( FIXED_MOVE_UPDATE_INTERVAL );
		controllers_engine->proceed_controllers_post( FIXED_MOVE_UPDATE_INTERVAL );
		collision_engine->proceed_collisions( FIXED_MOVE_UPDATE_INTERVAL );
		fable_engine->proceed_fable( FIXED_MOVE_UPDATE_INTERVAL );

		lag -= FIXED_MOVE_UPDATE_INTERVAL;
		//m_timeManager.UpdateTimeLag( lag );

		END_PERFORMANCE_CHECK( FRAME_COMPUTING_TIME )
	}
}


/**
@brief Renderowanie sceny i interpolacja po³o¿eñ obiektów. Odpowiada równie¿ za prezentowanie powsta³ej sceny.

@param[in] lag Przeliczanie po³o¿eñ obiektów odbywa siê ze sta³ym interwa³em czasowym.
Zmienna okreœla, ile up³yne³o czasu od ostatniego przeliczenia po³o¿eñ.
@param[in] timeInterval Czas jaki up³yn¹³ od ostatniej klatki.
*/

void Engine::RenderScene( float lag, float timeInterval )
{
	float framePercent = lag / FIXED_MOVE_UPDATE_INTERVAL;

#ifdef _INTERPOLATE_POSITIONS
	START_PERFORMANCE_CHECK( INTERPOLATION_TIME )

	display_engine->interpolate_positions( framePercent );

	END_PERFORMANCE_CHECK( INTERPOLATION_TIME )
#endif

	START_PERFORMANCE_CHECK( RENDERING_TIME )

	//Renderujemy scenê oraz interfejs u¿ytkownika
	display_engine->BeginScene();

	display_engine->display_scene( timeInterval, framePercent );
	ui_engine->draw_GUI( timeInterval, framePercent );

	END_PERFORMANCE_CHECK( RENDERING_TIME )		///< Ze wzglêdu na V-sync test wykonujemy przed wywyo³aniem funkcji present.

	display_engine->EndScene();
}


/**
@brief Zwraca wskaŸnik na g³ówny render target. Render target jest tworzony w tej funkcji.

Funkcja stworzona dla edytora, który zamierza wyœwietlaæ zawartoœæ render targetu w swoim oknie.
U¿ywaæ rozs¹dnie.

@return WskaŸnik na render target.
*/
void* Engine::GetRenderTargetHandle( uint16 width, uint16 height )
{
	RenderTargetDescriptor descriptor;
	descriptor.allowShareResource = 1;
	descriptor.textureWidth = width;
	descriptor.textureHeight = height;
	descriptor.colorBuffFormat = ResourceFormat::RESOURCE_FORMAT_B8G8R8A8_UNORM;
	descriptor.textureType = TextureType::TEXTURE_TYPE_TEXTURE2D;
	descriptor.depthStencilFormat = DepthStencilFormat::DEPTH_STENCIL_FORMAT_D24_UNORM_S8_UINT;
	descriptor.usage = ResourceUsage::RESOURCE_USAGE_DEFAULT;

	RenderTargetObject* renderTarget = models_manager->CreateRenderTarget( EDITOR_RENDERTARGET_STRING, descriptor );
	display_engine->SetMainRenderTarget( renderTarget );
	display_engine->SetProjectionMatrix( XMConvertToRadians( 45 ), (float)width / (float)height, 1, 100000 );

	return m_graphicInitializer->GetRenderTargetHandle( renderTarget );
}


#ifndef __UNUSED

/**@brief Funkcja oblicza interwa³ czasowy jaki up³yn¹³ od ostatniej ramki.
 *Poza tym s¹ tu generowane eventy dotycz¹ce czasu, opóŸnieñ itp.
 @param[out] time_interval Zwraca interwa³ jaki up³yn¹³ od ostatniego wywo³ania.*/
void Engine::time_controller(float& time_interval)
{
	__int64 time_current;
	LARGE_INTEGER time_temp;
	QueryPerformanceCounter( &time_temp );
	time_current = time_temp.QuadPart;
	
	__int64 time_diff;
	time_diff = time_current - time_previous;
	time_interval = (float)time_diff / timer_frequency;

	lag += time_interval;

	//zliczanie FPSów
	elapsed_time += time_diff;
	if ( elapsed_time >= FRAMES_PER_SEC_UPDATE * timer_frequency )	//aktualizujemy co 10 sekund
	{
		frames_per_sec = (float)frames / FRAMES_PER_SEC_UPDATE;	//FRAMES_PER_SEC_UPDATE w sekundach
		elapsed_time = elapsed_time % FRAMES_PER_SEC_UPDATE * timer_frequency;
		frames = 0;		//zerujemy liczbê klatek
	}

	//todo:	generujemy eventy czasowe
	
	//zapisujemy obecny czas i wychodzimy z funkcji
	time_previous = time_current;
	++frames;		//inkrementujemy licznik klatek
}

#endif

//----------------------------------------------------------------------------------------------//
//								funkcje pomocnicze												//
//----------------------------------------------------------------------------------------------//


/**@brief Funkcja pozwala wys³aæ event, który bêdzie potem przetworzony przez klase FableEngine.

@see Events
@param[in] new_event Event do wys³ania.*/
void Engine::send_event(Event* new_event)
{
	events_queue->push(new_event);
}

/**@brief Funkcja wczytuj¹ca startowy level do silnika. Najczêœciej na tym etapie wczytuje siê tylko menu,
oraz wszytkie elementy, które s¹ przydatne na ka¿dym etapie gry.

Zawartoœæ klasy IGamePlay powinna byæ jak najmniejsza, poniewa¿ wszystkie te rzeczy s¹ wczytywane
zanim cokolwiek pojawi siê na ekranie. Z tego wzglêdu lepiej najpierw wczytaæ ma³o, ¿eby u¿ytkownik
ju¿ coœ zobaczy³, a potem dopiero wczytaæ resztê wyœwietlaj¹c jednoczeœnie pasek wczytywania.

Funkcja nie jest dostêpna w EngineInterface. S³u¿y do wczytania tylko pierwszej klasy IGamePlay
jaka istnieje, na póŸniejszych etapach gry robi siê to innymi funkcjami.

Level siê nie wczyta, je¿eli nie zainicjowano DirectXa. Funkcje tworz¹ce bufory, textury
i tym podobne rzeczy wymagaj¹ zainicjowanego kontekstu urz¹dzenia DirectX, dlatego 
na wszelki wypadek zawsze inicjalizacja powinna byæ wczeœniej.

@param[in] game_play Obiekt do wczytania, jako pocz¹tek gry.
@see IGamePlay*/
void Engine::set_entry_point( IGamePlay* game_play )
{
	if ( m_engineReady )
	{
		game_play->set_engine_and_fable( this, fable_engine );
		fable_engine->set_game_play( game_play );
		
		int result = game_play->load_level();
		if ( result )
		{//Tutaj mo¿e siê znaleŸæ obs³uga b³êdów

		}
	}
}

#ifndef __UNUSED
///@brief Nie bêdzie wczytywania z bibliotek DLL. Maj¹ one ddzieln¹ stertê i powoduje to problemy ze zwalnianiem
///pamiêci. Poza tym taka architektura nie nadaje siê do przeci¹¿ania operatorów new i delete.
void Engine::set_entry_point( const std::wstring dll_name )
{
	HINSTANCE dll_entry_point;
	dll_entry_point = LoadLibrary( dll_name.c_str() );

	if ( dll_entry_point != NULL )
	{
		IGamePlay* game_play;


		if ( directX_ready )
		{
			fable_engine->set_game_play( game_play );
			game_play->load_level( );
		}
	}
}
#endif