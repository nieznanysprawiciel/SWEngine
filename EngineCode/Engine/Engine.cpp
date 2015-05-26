/**@file Engine.cpp
@brief Plik zawiera definicje metod klasy Engine dotycz�cych inicjacji i zwalniania DirectXa
oraz g��wne funkcje do renderingu.
*/

#include "stdafx.h"
#include "Engine.h"
#include "ControllersEngine\ControllersEngine.h"



#include "memory_leaks.h"


const float FIXED_MOVE_UPDATE_INTERVAL = ((float)1 / (float)56);	///<Interwa�, po kt�rym nast�puje kolejne przeliczenie po�o�e� obiekt�w (w sekundach).



//----------------------------------------------------------------------------------------------//
//								konstruktor i destruktor										//
//----------------------------------------------------------------------------------------------//


Engine::Engine(HINSTANCE instance)
{

	// Dzi�ki tej zmiennej b�dzie mo�na wysy�a� eventy
	Object::set_engine( this );

#ifndef __UNUSED
	//Zmienna decyduje o konczeniu w�tk�w
	join_render_thread = false;
#endif

	events_queue = nullptr;

	full_screen = false;			//inicjalizacja jako false potrzebna w funkcji init_window
	directX_ready = false;			//jeszcze nie zainicjowali�my
	instance_handler = instance;

	// Initialize global strings
	LoadString(instance_handler, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(instance_handler, IDC_SW_ENGINE, szWindowClass, MAX_LOADSTRING);

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
	frames = 0;
	frames_per_sec = 0;
	lag = 0.0;
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
int Engine::init_engine( int width, int height, BOOL full_screen, int nCmdShow )
{
	int result;

	//Tworzenie okna aplikacji
	result = init_window( width, height, full_screen, nCmdShow );
	if ( !result )
		return FALSE;

	//Inicjalizowanie directXa
	result = init_directX( );
	if ( result != GRAPHIC_ENGINE_INIT_OK )
		return FALSE;

	//Inicjalizowanie directXinputa
	result = ui_engine->init_direct_input( );
	if ( result != DIRECT_INPUT_OK )
	{
		clean_DirectX( );
		return FALSE;
	}
	//todo:
	//Inicjalizowanie DirectXSound

	directX_ready = true;		//jeste�my gotowi do renderowania

	return TRUE;
}



int Engine::init_directX()
{
	// Inicjujemy urz�dzenia. Ostatni parametr oznacza, �e b�dziemy u�ywa� zmiennej device z wielu w�tk�w
	int result = init_devices( window_width, window_height, window_handler, full_screen, false );
	
	if ( result != GRAPHIC_ENGINE_INIT_OK )
		return result;

	result = init_zBuffer( window_width, window_height );

	if ( result != GRAPHIC_ENGINE_INIT_OK )
		return result;
	//W przypadku niepowodzenia, wszystko jest zwalniane wewn�trz tamtych funkcji.
	//Je�eli do tej pory nic si� nie wywali�o to znaczy, �e mo�emy zacz�� robi� bardziej
	//z�o�one rzeczy.

	result = init_shaders_vertex_layouts();
	if ( result != GRAPHIC_ENGINE_INIT_OK )
		return result;

	display_engine->init_const_buffers();

	display_engine->set_projection_matrix( XMConvertToRadians( 45 ),
										   (float)window_width / (float)window_height, 1, 100000 );

	// W tej funkcji s� tworzone domy�lne shadery, materia�y itp.
	// Przekazujemy shadery, kt�re ju� stworzyli�my
	models_manager->set_default_assets( default_vertex_shader, default_pixel_shader );
	default_vertex_shader = nullptr;
	default_pixel_shader = nullptr;
	// Poniewa� oddali�my te obiekty do zarz�dzania ModelsManagerowi, to musimy skasowa� odwo�ania, �eby
	// nie zwalnia� obiekt�w dwukrotnie. Nie jest to eleganckie, no ale lepiej mie� wszystko w jednym miejscu.

	return GRAPHIC_ENGINE_INIT_OK;
}


int Engine::init_shaders_vertex_layouts( )
{
	HRESULT result;

	result = init_vertex_shader( L"shaders/default_shaders.fx", "vertex_shader" );
	if ( FAILED( result ) )
	{//Musimy sami zwalnia�, bo funkcje tego nie robi�
		release_DirectX( );
		return result;
	}

	result = init_pixel_shader( L"shaders/default_shaders.fx", "pixel_shader" );
	if ( FAILED( result ) )
	{
		release_DirectX( );
		return result;
	}

	result = init_mesh_vertex_format( VertexNormalTexCord1_desc,
									  VertexNormalTexCord1_desc_num_of_elements,
									  compiled_vertex_shader );
	if ( FAILED( result ) )
	{
		release_DirectX( );
		return result;
	}

	// Na razie nie mo�emy tego zrobi�, bo podawany shader musi mie� zgodne wej�cie z layoutem
	/*result = init_ui_vertex_format( VertexTexCord1_desc,
									VertexTexCord1_desc_num_of_elements,
									compiled_vertex_shader );

	if ( FAILED( result ) )
	{
		release_DirectX( );
		return result;
	}*/

	return GRAPHIC_ENGINE_INIT_OK;
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
	float time_interval;
	time_controller( time_interval );


	while ( lag >= FIXED_MOVE_UPDATE_INTERVAL )
	{
		ui_engine->proceed_input( FIXED_MOVE_UPDATE_INTERVAL );
		physic_engine->proceed_physic( FIXED_MOVE_UPDATE_INTERVAL );
		controllers_engine->proceed_controllers_pre( FIXED_MOVE_UPDATE_INTERVAL );
		movement_engine->proceed_movement( FIXED_MOVE_UPDATE_INTERVAL );
		controllers_engine->proceed_controllers_post( FIXED_MOVE_UPDATE_INTERVAL );
		collision_engine->proceed_collisions( FIXED_MOVE_UPDATE_INTERVAL );
		fable_engine->proceed_fable( FIXED_MOVE_UPDATE_INTERVAL );

		lag -= FIXED_MOVE_UPDATE_INTERVAL;
	}


#ifdef _INTERPOLATE_POSITIONS
	display_engine->interpolate_positions( lag / FIXED_MOVE_UPDATE_INTERVAL );
#endif

	//Renderujemy scen� oraz interfejs u�ytkownika
	begin_scene();

	display_engine->display_scene( time_interval, lag / FIXED_MOVE_UPDATE_INTERVAL );
	ui_engine->draw_GUI( time_interval, lag / FIXED_MOVE_UPDATE_INTERVAL );

	end_scene_and_present();
}




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
	if ( directX_ready )
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