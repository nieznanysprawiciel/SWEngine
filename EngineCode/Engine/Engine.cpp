#include "..\stdafx.h"
#include "Engine.h"
#include "ControllersEngine\ControllersEngine.h"



//interwa³, po którym nastêpuje kolejne przeliczenie po³o¿eñ obiektów (w sekundach)
const float FIXED_MOVE_UPDATE_INTERVAL = ((float)1 / (float)56);


Engine::Engine(HINSTANCE instance)
{
	directX_interface = nullptr;
	directX_device = nullptr;

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

	//dziêki tej zmiennej bêdzie mo¿na wysy³aæ eventy
	Object::engine = this;

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
	delete ui_engine;
	delete models_manager;		//musi byæ kasowany na koñcu

	clean_DirectX();
}




int Engine::init_directX()
{
	directX_interface = Direct3DCreate9(D3D_SDK_VERSION);    // create the Direct3D interface

	D3DPRESENT_PARAMETERS d3dpp;    // create a struct to hold various device information

	ZeroMemory(&d3dpp, sizeof(d3dpp));    // clear out the struct for use

	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;    // discard old frames
	d3dpp.hDeviceWindow = window_handler;		// set the window to be used by Direct3D
	d3dpp.EnableAutoDepthStencil = TRUE;		//z - buffer
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24X8;					//format z - buffera
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;		//bufor jest odœwie¿any niezale¿nie od czêstotliwoœci monitora

	if (full_screen)
	{
		d3dpp.Windowed = FALSE;
		d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;    // set the back buffer format to 32-bit
		d3dpp.BackBufferWidth = window_width;    // set the width of the buffer
		d3dpp.BackBufferHeight = window_height;    // set the height of the buffer
	}
	else
	{
		d3dpp.Windowed = TRUE;    // program windowed, not fullscreen
	}

	// create a device class using this information and information from the d3dpp stuct
	int result = directX_interface->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		window_handler,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&directX_device);

	if (result == GRAPHIC_ENGINE_INIT_OK)
	{
		directX_ready = true;
		//przy okazji inicjujemy macierz projekcji jakimiœ domyœlnymi wartoœciami
		display_engine->set_projection_matrix(D3DXToRadian(45),
			(float)window_width / (float)window_height, 1, 100000);
		//w³¹czamy z - buffer
		directX_device->SetRenderState(D3DRS_ZENABLE, TRUE);
		directX_device->SetRenderState( D3DRS_LIGHTING, TRUE );
	}

	return result;
}

int Engine::init_directXinput()
{
	return ui_engine->init_direct_input();
}


void Engine::clean_DirectX()
{
	// close and release all existing COM objects
	if (directX_interface != nullptr)
		directX_interface->Release();
	if (directX_device != nullptr)
		directX_device->Release();
}

/*To tutaj dziej¹ siê wszystkie rzeczy, które s¹ wywo³ywane co ka¿d¹ klatkê*/
void Engine::render_frame()
{
	float time_interval;
	time_controller( time_interval );


#ifdef FIXED_FRAMES_COUNT
	while ( lag >= FIXED_MOVE_UPDATE_INTERVAL )
	{
		ui_engine->proceed_input( time_interval );
		physic_engine->proceed_physic( FIXED_MOVE_UPDATE_INTERVAL );
		controllers_engine->proceed_controllers_pre( FIXED_MOVE_UPDATE_INTERVAL );
		movement_engine->proceed_movement( FIXED_MOVE_UPDATE_INTERVAL );
		controllers_engine->proceed_controllers_post( FIXED_MOVE_UPDATE_INTERVAL );
		collision_engine->proceed_collisions( FIXED_MOVE_UPDATE_INTERVAL );
		fable_engine->proceed_fable( FIXED_MOVE_UPDATE_INTERVAL );

		lag -= FIXED_MOVE_UPDATE_INTERVAL;
	}
#else
	ui_engine->proceed_input( time_interval );
	physic_engine->proceed_physic(time_interval);
	controllers_engine->proceed_controllers_pre(time_interval);
	movement_engine->proceed_movement(time_interval);
	controllers_engine->proceed_controllers_post(time_interval);
	collision_engine->proceed_collisions(time_interval);
	fable_engine->proceed_fable(time_interval);

#endif

#ifdef _INTERPOLATE_POSITIONS
	display_engine->interpolate_positions(lag);
#endif

	//Renderujemy scenê oraz interfejs u¿ytkownika
	directX_device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
	directX_device->Clear( 0, nullptr, D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB( 0, 0, 0 ), 1.0f, 0 );
	directX_device->BeginScene();    // begins the 3D scene

	display_engine->display_scene(time_interval);
	ui_engine->draw_GUI(time_interval);

	directX_device->EndScene();    // ends the 3D scene
	directX_device->Present(NULL, NULL, NULL, NULL);    // displays the created frame
}




/*Funkcja oblicza interwa³ czasowy jaki up³yn¹³ od ostatniej ramki.
 *Poza tym s¹ tu generowane eventy dotycz¹ce czasu, opóŸnieñ itp.*/
void Engine::time_controller(float& time_interval)
{
	DWORD time_current = GetTickCount();
	
	DWORD time_diff;
	time_diff = time_current - time_previous;
	time_interval = (float)time_diff / 1000;	//funkcja zwraca czas z dok³adnoœci¹ do milisekund, przeliczamy na sekundy

	lag += time_interval;

	//zliczanie FPSów
	elapsed_time += time_diff;
	if ( elapsed_time >= FRAMES_PER_SEC_UPDATE )	//aktualizujemy co 10 sekund
	{
		frames_per_sec = (float)frames / FRAMES_PER_SEC_UPDATE * 1000;	//FRAMES_PER_SEC_UPDATE w milisekundach wiêc mno¿ymy przez 1000
		elapsed_time = elapsed_time % FRAMES_PER_SEC_UPDATE;
		frames = 0;		//zerujemy liczbê klatek
	}

	//todo:	generujemy eventy czasowe
	
	//zapisujemy obecny czas i wychodzimy z funkcji
	time_previous = time_current;
	++frames;		//inkrementujemy licznik klatek
}

/*Funkcja pozwala wys³aæ event, który bêdzie potem przetworzony przez klase FableEngine.
 *Eventy s¹ metod¹ komunikacji pomiedzy silnikiem graficznym, silnikiem fizycznym, AI i silnikiem kolizji,
 *a modu³em silnika odpowiedzialnym za fabu³ê. Istnieje szereg eventów wbudowanych, wysy³anych przez silnik,
 *mo¿na równie¿ definiowaæ w³asne nowe eventy poprzez dziedziczenie z klasy Event. Event mo¿e byæ wys³any przez dowolny
 *objekt poprzez wywo³anie funkcji Object::event. Aby wys³aæ w³asny event trzeba przeci¹¿yæ jedn¹ z funkcji klas wbudowanych,
 *która jest potem wywo³ywana przez silnik i wywo³aæ tê funkjê.
 *
 *Za zwolnienie pamiêci po klasie Event odpowiada klasa FabelEngine (jest to robione automatycznie po wywo³aniu funkcji obs³ugi,
 *u¿ytkownik nie musi siê tym przejmowac).*/
void Engine::send_event(Event* new_event)
{
	events_queue->push(new_event);
}

/*
Funkcja wczytuj¹ca startowy level do silnika. Najczêœciej na tym etapie wczytuje siê tylko menu,
oraz wszytkie elementy, które s¹ przydatne na ka¿dym etapie gry.

Zawartosc klasy GamePlay powinna byæ jak najmniejsza, poniewa¿ wszystkie te rzeczy s¹ wczytywane
zanim cokolwiek pojawi siê na ekranie. Z tego wzglêdu lepiej najpierw wczytaæ ma³o, ¿eby u¿ytkownik
ju¿ coœ zobaczy³, a potem dopiero wczytaæ resztê wyœwietlaj¹c jednoczeœnie pasek wczytywania.

Funkcja nie jest dostêpna w EngineInterface. S³u¿y do wczytania tylko pierwszej klasy GamePlay
jaka istnieje, na póŸniejszych etapach gry robi siê to innymi funkcjami.

Level siê nie wczyta, je¿eli nie zainicjowano DirectXa. Funkcje tworz¹ce bufory, textury
i tym podobne rzeczy wymagaj¹ zainicjowanego kontekstu urz¹dzenia DirectX, dlatego 
na wszelki wypadek zawsze inicjalizacja powinna byæ wczeœniej.*/
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
