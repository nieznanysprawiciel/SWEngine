/**@file Engine.cpp
@brief Plik zawiera definicje metod klasy Engine dotycz�cych inicjacji i zwalniania DirectXa
oraz g��wne funkcje do renderingu.
*/


#include "stdafx.h"
#include "Engine.h"
#include "ControllersEngine/ControllersEngine.h"
#include "GraphicAPI/ResourcesFactory.h"
#include "EngineHelpers/PerformanceCheck.h"

#include "Common/memory_leaks.h"




const float FIXED_MOVE_UPDATE_INTERVAL = ((float)1 / (float)56);	///<Interwa�, po kt�rym nast�puje kolejne przeliczenie po�o�e� obiekt�w (w sekundach).

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

	// Dzi�ki tej zmiennej b�dzie mo�na wysy�a� eventy
	Object::set_engine( this );

#ifndef __UNUSED
	//Zmienna decyduje o konczeniu w�tk�w
	join_render_thread = false;
#endif

	events_queue = nullptr;

	full_screen = false;			//inicjalizacja jako false potrzebna w funkcji init_window
	m_engineReady = false;			//jeszcze nie zainicjowali�my
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
	//obiekty trzeba pokasowa�, zanim si� skasuje to, do czego si� odwo�uj�
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
	delete ui_engine;			//sprz�ta po directinpucie
	delete models_manager;		//musi by� kasowany na ko�cu

	clean_DirectX();

}


//----------------------------------------------------------------------------------------------//
//								inicjalizacja okna i DirectXa									//
//----------------------------------------------------------------------------------------------//

///@brief Inicjuje dzia�anie silnika.
///W trybie fullscreen szeroko�� i wysoko�� okna jest ignorowana, a dane s� pobierane z systemu.
///@param[in] width Szeroko�� okna
///@param[in] height Wysoko�� okna
///@param[in] fullscreen Pe�ny ekran lub renderowanie w oknie
///@param[in] nCmdShow Czwarty parametr funkcji WinMain
int Engine::init_engine( int width, int height, bool full_screen, int nCmdShow )
{
	int result;

	//Tworzenie okna aplikacji
	result = init_window( width, height, full_screen, nCmdShow );
	if ( !result )
		return FALSE;

	//Inicjalizowanie API graficznego
	GraphicAPIInitData initData;
	initData.fullScreen			= full_screen;
	initData.singleThreaded		= false;
	initData.windowHandle		= (uint32)window_handler;
	initData.windowHeight		= height;
	initData.windowWidth		= width;
	result = m_graphicInitializer->InitAPI( initData );
	assert( result != 0 );
	if( result == 0 )
		return FALSE;

	//Inicjalizowanie directXinputa
	result = ui_engine->init_direct_input( );
		assert( result == DIRECT_INPUT_OK );	//Dzia�a tylko w trybie DEBUG
	if ( result != DIRECT_INPUT_OK )
	{
		clean_DirectX( );
		return FALSE;
	}
	//todo:
	//Inicjalizowanie DirectXSound

	m_engineReady = true;		//jeste�my gotowi do renderowania

	return TRUE;
}



int Engine::init_directX()
{
	set_vertex_layout( DX11_DEFAULT_VERTEX_LAYOUT::VERTEX_NORMAL_TEXTURE );
	set_depth_stencil_format( DXGI_FORMAT_D24_UNORM_S8_UINT );		// Je�eli ma by� inny to trzeba to jawnie zmieni�.

	DX11_INIT_RESULT result = init_DX11( window_width, window_height, window_handler, full_screen,
										 L"shaders/default_shaders.fx", "pixel_shader",
										 L"shaders/default_shaders.fx", "vertex_shader", false );

	if ( result != DX11_INIT_OK )
		return result;


	display_engine->InitRenderer();

	display_engine->SetProjectionMatrix( XMConvertToRadians( 45 ),
										   (float)window_width / (float)window_height, 1, 100000 );

	// W tej funkcji s� tworzone domy�lne shadery, materia�y itp.
	// Przekazujemy shadery, kt�re ju� stworzyli�my
	models_manager->set_default_assets( default_vertex_shader, default_pixel_shader );
	default_vertex_shader = nullptr;
	default_pixel_shader = nullptr;
	// Poniewa� oddali�my te obiekty do zarz�dzania ModelsManagerowi, to musimy skasowa� odwo�ania, �eby
	// nie zwalnia� obiekt�w dwukrotnie. Nie jest to eleganckie, no ale lepiej mie� wszystko w jednym miejscu.

	return DX11_INIT_OK;
}


int Engine::init_directXinput()
{
	return ui_engine->init_direct_input();
}


//----------------------------------------------------------------------------------------------//
//								czyszczenie po DirectX											//
//----------------------------------------------------------------------------------------------//

void Engine::clean_DirectX()
{
	release_DirectX();
}


//----------------------------------------------------------------------------------------------//
//								potok przetwarzania obiekt�w									//
//----------------------------------------------------------------------------------------------//

/**@brief To tutaj dziej� si� wszystkie rzeczy, kt�re s� wywo�ywane co ka�d� klatk�.*/
void Engine::render_frame()
{
	float time_interval = time_manager.onStartRenderFrame();
	float lag = time_manager.getTimeLag();


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
		time_manager.updateTimeLag( lag );

		END_PERFORMANCE_CHECK( FRAME_COMPUTING_TIME )
	}


#ifdef _INTERPOLATE_POSITIONS
	START_PERFORMANCE_CHECK( INTERPOLATION_TIME )

	display_engine->interpolate_positions( lag / FIXED_MOVE_UPDATE_INTERVAL );

	END_PERFORMANCE_CHECK( INTERPOLATION_TIME )
#endif

	START_PERFORMANCE_CHECK(RENDERING_TIME)

	//Renderujemy scen� oraz interfejs u�ytkownika
	begin_scene();

	display_engine->display_scene( time_interval, lag / FIXED_MOVE_UPDATE_INTERVAL );
	ui_engine->draw_GUI( time_interval, lag / FIXED_MOVE_UPDATE_INTERVAL );

	END_PERFORMANCE_CHECK( RENDERING_TIME )		///< Ze wzgl�du na V-sync test wykonujemy przed wywyo�aniem funkcji present.

	end_scene_and_present();
}


#ifndef __UNUSED

/**@brief Funkcja oblicza interwa� czasowy jaki up�yn�� od ostatniej ramki.
 *Poza tym s� tu generowane eventy dotycz�ce czasu, op�nie� itp.
 @param[out] time_interval Zwraca interwa� jaki up�yn�� od ostatniego wywo�ania.*/
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

	//zliczanie FPS�w
	elapsed_time += time_diff;
	if ( elapsed_time >= FRAMES_PER_SEC_UPDATE * timer_frequency )	//aktualizujemy co 10 sekund
	{
		frames_per_sec = (float)frames / FRAMES_PER_SEC_UPDATE;	//FRAMES_PER_SEC_UPDATE w sekundach
		elapsed_time = elapsed_time % FRAMES_PER_SEC_UPDATE * timer_frequency;
		frames = 0;		//zerujemy liczb� klatek
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


/**@brief Funkcja pozwala wys�a� event, kt�ry b�dzie potem przetworzony przez klase FableEngine.

@see Events
@param[in] new_event Event do wys�ania.*/
void Engine::send_event(Event* new_event)
{
	events_queue->push(new_event);
}

/**@brief Funkcja wczytuj�ca startowy level do silnika. Najcz�ciej na tym etapie wczytuje si� tylko menu,
oraz wszytkie elementy, kt�re s� przydatne na ka�dym etapie gry.

Zawarto�� klasy GamePlay powinna by� jak najmniejsza, poniewa� wszystkie te rzeczy s� wczytywane
zanim cokolwiek pojawi si� na ekranie. Z tego wzgl�du lepiej najpierw wczyta� ma�o, �eby u�ytkownik
ju� co� zobaczy�, a potem dopiero wczyta� reszt� wy�wietlaj�c jednocze�nie pasek wczytywania.

Funkcja nie jest dost�pna w EngineInterface. S�u�y do wczytania tylko pierwszej klasy GamePlay
jaka istnieje, na p�niejszych etapach gry robi si� to innymi funkcjami.

Level si� nie wczyta, je�eli nie zainicjowano DirectXa. Funkcje tworz�ce bufory, textury
i tym podobne rzeczy wymagaj� zainicjowanego kontekstu urz�dzenia DirectX, dlatego 
na wszelki wypadek zawsze inicjalizacja powinna by� wcze�niej.

@param[in] game_play Obiekt do wczytania, jako pocz�tek gry.
@see GamePlay*/
void Engine::set_entry_point( GamePlay* game_play )
{
	if ( m_engineReady )
	{
		game_play->set_engine_and_fable( this, fable_engine );
		fable_engine->set_game_play( game_play );
		
		int result = game_play->load_level();
		if ( result )
		{//Tutaj mo�e si� znale�� obs�uga b��d�w

		}
	}
}

#ifndef __UNUSED
///@brief Nie b�dzie wczytywania z bibliotek DLL. Maj� one ddzieln� stert� i powoduje to problemy ze zwalnianiem
///pami�ci. Poza tym taka architektura nie nadaje si� do przeci��ania operator�w new i delete.
void Engine::set_entry_point( const std::wstring dll_name )
{
	HINSTANCE dll_entry_point;
	dll_entry_point = LoadLibrary( dll_name.c_str() );

	if ( dll_entry_point != NULL )
	{
		GamePlay* game_play;


		if ( directX_ready )
		{
			fable_engine->set_game_play( game_play );
			game_play->load_level( );
		}
	}
}
#endif