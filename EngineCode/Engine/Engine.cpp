/**@file Engine.cpp
@brief Plik zawiera definicje metod klasy Engine dotycz¹cych inicjacji i zwalniania DirectXa
oraz g³ówne funkcje do renderingu.
*/

#include "stdafx.h"
#include "Engine.h"
#include "ControllersEngine\ControllersEngine.h"



#include "memory_leaks.h"


const float FIXED_MOVE_UPDATE_INTERVAL = ((float)1 / (float)56);	///<Interwa³, po którym nastêpuje kolejne przeliczenie po³o¿eñ obiektów (w sekundach).



//----------------------------------------------------------------------------------------------//
//								konstruktor i destruktor										//
//----------------------------------------------------------------------------------------------//


Engine::Engine(HINSTANCE instance)
{

	// Dziêki tej zmiennej bêdzie mo¿na wysy³aæ eventy
	Object::set_engine( this );

#ifndef __UNUSED
	//Zmienna decyduje o konczeniu w¹tków
	join_render_thread = false;
#endif

	events_queue = nullptr;

	full_screen = false;			//inicjalizacja jako false potrzebna w funkcji init_window
	directX_ready = false;			//jeszcze nie zainicjowaliœmy
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

	clean_DirectX();
}


//----------------------------------------------------------------------------------------------//
//								inicjalizacja okna i DirectXa									//
//----------------------------------------------------------------------------------------------//

///@brief Inicjuje dzia³anie silnika.
///W trybie fullscreen szerokoœæ i wysokoœæ okna jest ignorowana, a dane s¹ pobierane z systemu.
///@param[in] width Szerokoœæ okna
///@param[in] height Wysokoœæ okna
///@param[in] fullscreen Pe³ny ekran lub renderowanie w oknie
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

	directX_ready = true;		//jesteœmy gotowi do renderowania

	return TRUE;
}



int Engine::init_directX()
{
	// Inicjujemy urz¹dzenia. Ostatni parametr oznacza, ¿e bêdziemy u¿ywaæ zmiennej device z wielu w¹tków
	int result = init_devices( window_width, window_height, window_handler, full_screen, false );
	
	if ( result != GRAPHIC_ENGINE_INIT_OK )
		return result;

	result = init_zBuffer( window_width, window_height );

	if ( result != GRAPHIC_ENGINE_INIT_OK )
		return result;
	//W przypadku niepowodzenia, wszystko jest zwalniane wewn¹trz tamtych funkcji.
	//Je¿eli do tej pory nic siê nie wywali³o to znaczy, ¿e mo¿emy zacz¹æ robiæ bardziej
	//z³o¿one rzeczy.

	result = init_shaders_vertex_layouts();
	if ( result != GRAPHIC_ENGINE_INIT_OK )
		return result;

	display_engine->init_const_buffers();

	display_engine->set_projection_matrix( XMConvertToRadians( 45 ),
										   (float)window_width / (float)window_height, 1, 100000 );

	// W tej funkcji s¹ tworzone domyœlne shadery, materia³y itp.
	// Przekazujemy shadery, które ju¿ stworzyliœmy
	models_manager->set_default_assets( default_vertex_shader, default_pixel_shader );
	default_vertex_shader = nullptr;
	default_pixel_shader = nullptr;
	// Poniewa¿ oddaliœmy te obiekty do zarz¹dzania ModelsManagerowi, to musimy skasowaæ odwo³ania, ¿eby
	// nie zwalniaæ obiektów dwukrotnie. Nie jest to eleganckie, no ale lepiej mieæ wszystko w jednym miejscu.

	return GRAPHIC_ENGINE_INIT_OK;
}


int Engine::init_shaders_vertex_layouts( )
{
	HRESULT result;

	result = init_vertex_shader( L"shaders/default_shaders.fx", "vertex_shader" );
	if ( FAILED( result ) )
	{//Musimy sami zwalniaæ, bo funkcje tego nie robi¹
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

	// Na razie nie mo¿emy tego zrobiæ, bo podawany shader musi mieæ zgodne wejœcie z layoutem
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
//								potok przetwarzania obiektów									//
//----------------------------------------------------------------------------------------------//

/**@brief To tutaj dziej¹ siê wszystkie rzeczy, które s¹ wywo³ywane co ka¿d¹ klatkê.*/
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

	//Renderujemy scenê oraz interfejs u¿ytkownika
	begin_scene();

	display_engine->display_scene( time_interval, lag / FIXED_MOVE_UPDATE_INTERVAL );
	ui_engine->draw_GUI( time_interval, lag / FIXED_MOVE_UPDATE_INTERVAL );

	end_scene_and_present();
}




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

Zawartoœæ klasy GamePlay powinna byæ jak najmniejsza, poniewa¿ wszystkie te rzeczy s¹ wczytywane
zanim cokolwiek pojawi siê na ekranie. Z tego wzglêdu lepiej najpierw wczytaæ ma³o, ¿eby u¿ytkownik
ju¿ coœ zobaczy³, a potem dopiero wczytaæ resztê wyœwietlaj¹c jednoczeœnie pasek wczytywania.

Funkcja nie jest dostêpna w EngineInterface. S³u¿y do wczytania tylko pierwszej klasy GamePlay
jaka istnieje, na póŸniejszych etapach gry robi siê to innymi funkcjami.

Level siê nie wczyta, je¿eli nie zainicjowano DirectXa. Funkcje tworz¹ce bufory, textury
i tym podobne rzeczy wymagaj¹ zainicjowanego kontekstu urz¹dzenia DirectX, dlatego 
na wszelki wypadek zawsze inicjalizacja powinna byæ wczeœniej.

@param[in] game_play Obiekt do wczytania, jako pocz¹tek gry.
@see GamePlay*/
void Engine::set_entry_point( GamePlay* game_play )
{
	if ( directX_ready )
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
		GamePlay* game_play;


		if ( directX_ready )
		{
			fable_engine->set_game_play( game_play );
			game_play->load_level( );
		}
	}
}
#endif