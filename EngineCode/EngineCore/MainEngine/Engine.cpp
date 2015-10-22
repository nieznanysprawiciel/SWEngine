/**@file Engine.cpp
@author nieznanysprawiciel
@copyright Plik jest cz�ci� silnika graficznego SWEngine.

@brief Plik zawiera definicje metod klasy Engine dotycz�cych inicjacji i zwalniania DirectXa
oraz g��wne funkcje do renderingu.
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

	// Dzi�ki tej zmiennej b�dzie mo�na wysy�a� eventy
	Object::set_engine( this );

#ifndef __UNUSED
	//Zmienna decyduje o konczeniu w�tk�w
	join_render_thread = false;
#endif

	Context.eventsQueue = nullptr;

	Context.fullScreen = false;			//inicjalizacja jako false potrzebna w funkcji init_window
	Context.engineReady = false;			//jeszcze nie zainicjowali�my
	Context.instanceHandler = instance;

	// Initialize global strings
	LoadString(Context.instanceHandler, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(Context.instanceHandler, IDC_SW_ENGINE, szWindowClass, MAX_LOADSTRING);

	Context.graphicInitializer = ResourcesFactory::CreateAPIInitializer();

	Context.controllersEngine	=	new ControllersEngine(this);
	Context.movementEngine		=	new MovementEngine(this);
	Context.displayEngine		=	new DisplayEngine(this);
	Context.collisionEngine	=	new CollisionEngine(this);
	Context.physicEngine		=	new PhysicEngine(this);
	Context.modelsManager		=	new ModelsManager(this);
	Context.fableEngine		=	new FableEngine(this);
	Context.soundEngine		=	new SoundEngine(this);
	Context.ui_engine			=	new UI_Engine(this);

	//inicjujemy licznik klatek
	Context.pause = false;
}


Engine::~Engine()
{

#ifdef __TEST
	//obiekty trzeba pokasowa�, zanim si� skasuje to, do czego si� odwo�uj�
	for ( unsigned int i = 0; i < object_list.size(); ++i )
		delete object_list[i];
#endif

	delete Context.controllersEngine;
	delete Context.movementEngine;
	delete Context.displayEngine;
	delete Context.collisionEngine;
	delete Context.physicEngine;
	delete Context.fableEngine;
	delete Context.soundEngine;
	delete Context.ui_engine;			//sprz�ta po directinpucie
	delete Context.modelsManager;		//musi by� kasowany na ko�cu

	DefaultAssets::Release();
	Context.graphicInitializer->ReleaseAPI();
	delete Context.graphicInitializer;
}


//----------------------------------------------------------------------------------------------//
//								inicjalizacja okna i modu��w zewn�trznych						//
//----------------------------------------------------------------------------------------------//

///@brief Inicjuje dzia�anie silnika.
///W trybie fullscreen szeroko�� i wysoko�� okna jest ignorowana, a dane s� pobierane z systemu.
///@param[in] width Szeroko�� okna
///@param[in] height Wysoko�� okna
///@param[in] fullscreen Pe�ny ekran lub renderowanie w oknie
///@param[in] nCmdShow Czwarty parametr funkcji WinMain. Decyduje w jaki spos�b powinno zosta� pokazane okno aplikacji.
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

	// Czym p�niej zainicjujemy tym lepiej.
	Context.timeManager.InitTimer();

	Context.engineReady = true;		//jeste�my gotowi do renderowania

	return TRUE;
}



bool Engine::InitGraphicAPI( int width, int height, bool full_screen )
{
	bool result;

	//Inicjalizowanie API graficznego
	GraphicAPIInitData initData;
	initData.fullScreen			= full_screen;
	initData.singleThreaded		= false;
	initData.windowHandle		= (uint32)Context.windowHandler;
	initData.windowHeight		= height;
	initData.windowWidth		= width;
	initData.depthStencilFormat = ResourceFormat::RESOURCE_FORMAT_D24_UNORM_S8_UINT;
	result = Context.graphicInitializer->InitAPI( initData );
	assert( result != 0 );
	if( result == 0 )
		return false;
	return true;
}

bool Engine::InitInputModule		()
{
	int result;

	//Inicjalizowanie directXinputa
	result = Context.ui_engine->init_direct_input( );
		assert( result == DIRECT_INPUT_OK );	//Dzia�a tylko w trybie DEBUG
	if ( result != DIRECT_INPUT_OK )
		return false;
	return true;
}

bool Engine::InitSoundModule		()
{
	return true;
}

//----------------------------------------------------------------------------------------------//
//								Przygotowanie modu��w do dzia�ania								//
//----------------------------------------------------------------------------------------------//

/**@brief Inicjuje domy�lne assety silnika.*/
bool Engine::InitDefaultAssets()
{
	DefaultAssets::Init();

	ShaderInputLayoutObject* layout;
	Context.modelsManager->AddVertexShader( DEFAULT_VERTEX_SHADER_STRING, DEFAULT_VERTEX_SHADER_ENTRY, &layout, DefaultAssets::LAYOUT_POSITION_NORMAL_COORD );
	Context.modelsManager->AddPixelShader( DEFAULT_PIXEL_SHADER_STRING, DEFAULT_PIXEL_SHADER_ENTRY );
	Context.modelsManager->AddPixelShader( DEFAULT_TEX_DIFFUSE_PIXEL_SHADER_PATH, DEFAULT_PIXEL_SHADER_ENTRY );

	Context.displayEngine->SetLayout( layout );		///@todo Hack. Layout powinien by� ustawialny dla ka�dego mesha z osobna. Zlikwidowa�.

	MaterialObject* nullMaterial = new MaterialObject();
	nullMaterial->SetNullMaterial();
	Context.modelsManager->AddMaterial( nullMaterial, DEFAULT_MATERIAL_STRING );

	RenderTargetObject* mainRenderTarget = ResourcesFactory::CreateScreenRenderTarget();
	Context.modelsManager->AddRenderTarget( mainRenderTarget, SCREEN_RENDERTARGET_STRING );

	return true;
}

/**@brief Inicjalizuje DisplayEngine.

Funkcja tworzy renderery, domy�lne bufory sta�ych oraz ustawia macierz projekcji.
@return Zwraca zawsze true.*/
bool Engine::InitDisplayer()
{
	IRenderer* renderer = Context.graphicInitializer->CreateRenderer( RendererUsage::USE_AS_IMMEDIATE );
	Context.displayEngine->InitRenderer( renderer );
	Context.displayEngine->InitDisplayer( Context.modelsManager );

	Context.displayEngine->SetProjectionMatrix( XMConvertToRadians( 45 ),
										   (float)Context.windowWidth / (float)Context.windowHeight, 1, 100000 );

	return true;
}



//----------------------------------------------------------------------------------------------//
//								potok przetwarzania obiekt�w									//
//----------------------------------------------------------------------------------------------//

/**@brief To tutaj dziej� si� wszystkie rzeczy, kt�re s� wywo�ywane co ka�d� klatk�.*/
void Engine::RenderFrame()
{
	float time_interval = Context.timeManager.onStartRenderFrame();
	float lag = Context.timeManager.GetFrameLag();


	while ( lag >= FIXED_MOVE_UPDATE_INTERVAL )
	{
		START_PERFORMANCE_CHECK(FRAME_COMPUTING_TIME)

		Context.ui_engine->proceed_input( FIXED_MOVE_UPDATE_INTERVAL );
		Context.physicEngine->proceed_physic( FIXED_MOVE_UPDATE_INTERVAL );
		Context.controllersEngine->proceed_controllers_pre( FIXED_MOVE_UPDATE_INTERVAL );
		Context.movementEngine->proceed_movement( FIXED_MOVE_UPDATE_INTERVAL );
		Context.controllersEngine->proceed_controllers_post( FIXED_MOVE_UPDATE_INTERVAL );
		Context.collisionEngine->proceed_collisions( FIXED_MOVE_UPDATE_INTERVAL );
		Context.fableEngine->proceed_fable( FIXED_MOVE_UPDATE_INTERVAL );

		lag -= FIXED_MOVE_UPDATE_INTERVAL;
		Context.timeManager.UpdateTimeLag( lag );

		END_PERFORMANCE_CHECK( FRAME_COMPUTING_TIME )
	}


#ifdef _INTERPOLATE_POSITIONS
	START_PERFORMANCE_CHECK( INTERPOLATION_TIME )

	Context.displayEngine->interpolate_positions( lag / FIXED_MOVE_UPDATE_INTERVAL );

	END_PERFORMANCE_CHECK( INTERPOLATION_TIME )
#endif

	START_PERFORMANCE_CHECK( RENDERING_TIME )

	//Renderujemy scen� oraz interfejs u�ytkownika
	Context.displayEngine->BeginScene();

	Context.displayEngine->display_scene( time_interval, lag / FIXED_MOVE_UPDATE_INTERVAL );
	Context.ui_engine->draw_GUI( time_interval, lag / FIXED_MOVE_UPDATE_INTERVAL );

	END_PERFORMANCE_CHECK( RENDERING_TIME )		///< Ze wzgl�du na V-sync test wykonujemy przed wywyo�aniem funkcji present.

	Context.displayEngine->EndScene();
}



/**
@brief Aktualizuje po�o�enia obiekt�w.

Wywo�ywane s� funkcje odpowiadaj�ce za fizyk�, ruch, kontrolery, wej�cie od u�ytkownika i fabu��.

@param[inout] lag Przeliczanie po�o�e� obiekt�w odbywa si� ze sta�ym interwa�em czasowym.
Zmienna okre�la, ile up�yne�o czasu od ostatniego przeliczenia po�o�e�.
@param[in] timeInterval Czas jaki up�yn�� od ostatniej klatki.
*/
void Engine::UpdateScene( float& lag, float timeInterval )
{
	while ( lag >= FIXED_MOVE_UPDATE_INTERVAL )
	{
		START_PERFORMANCE_CHECK(FRAME_COMPUTING_TIME)

		Context.ui_engine->proceed_input( FIXED_MOVE_UPDATE_INTERVAL );
		Context.physicEngine->proceed_physic( FIXED_MOVE_UPDATE_INTERVAL );
		Context.controllersEngine->proceed_controllers_pre( FIXED_MOVE_UPDATE_INTERVAL );
		Context.movementEngine->proceed_movement( FIXED_MOVE_UPDATE_INTERVAL );
		Context.controllersEngine->proceed_controllers_post( FIXED_MOVE_UPDATE_INTERVAL );
		Context.collisionEngine->proceed_collisions( FIXED_MOVE_UPDATE_INTERVAL );
		Context.fableEngine->proceed_fable( FIXED_MOVE_UPDATE_INTERVAL );

		lag -= FIXED_MOVE_UPDATE_INTERVAL;
		//timeManager.UpdateTimeLag( lag );

		END_PERFORMANCE_CHECK( FRAME_COMPUTING_TIME )
	}
}


/**
@brief Renderowanie sceny i interpolacja po�o�e� obiekt�w. Odpowiada r�wnie� za prezentowanie powsta�ej sceny.

@param[in] lag Przeliczanie po�o�e� obiekt�w odbywa si� ze sta�ym interwa�em czasowym.
Zmienna okre�la, ile up�yne�o czasu od ostatniego przeliczenia po�o�e�.
@param[in] timeInterval Czas jaki up�yn�� od ostatniej klatki.
*/

void Engine::RenderScene( float lag, float timeInterval )
{
	float framePercent = lag / FIXED_MOVE_UPDATE_INTERVAL;

#ifdef _INTERPOLATE_POSITIONS
	START_PERFORMANCE_CHECK( INTERPOLATION_TIME )

	Context.displayEngine->interpolate_positions( framePercent );

	END_PERFORMANCE_CHECK( INTERPOLATION_TIME )
#endif

	START_PERFORMANCE_CHECK( RENDERING_TIME )

	//Renderujemy scen� oraz interfejs u�ytkownika
	Context.displayEngine->BeginScene();

	Context.displayEngine->display_scene( timeInterval, framePercent );
	Context.ui_engine->draw_GUI( timeInterval, framePercent );

	END_PERFORMANCE_CHECK( RENDERING_TIME )		///< Ze wzgl�du na V-sync test wykonujemy przed wywyo�aniem funkcji present.

	Context.displayEngine->EndScene();
}


/**
@brief Zwraca wska�nik na g��wny render target. Render target jest tworzony w tej funkcji.

Funkcja stworzona dla edytora, kt�ry zamierza wy�wietla� zawarto�� render targetu w swoim oknie.
U�ywa� rozs�dnie.

@return Wska�nik na render target.
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

	RenderTargetObject* renderTarget = Context.modelsManager->CreateRenderTarget( EDITOR_RENDERTARGET_STRING, descriptor );
	Context.displayEngine->SetMainRenderTarget( renderTarget );
	Context.displayEngine->SetProjectionMatrix( XMConvertToRadians( 45 ), (float)width / (float)height, 1, 100000 );

	return Context.graphicInitializer->GetRenderTargetHandle( renderTarget );
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
	Context.eventsQueue->push(new_event);
}

/**@brief Funkcja wczytuj�ca startowy level do silnika. Najcz�ciej na tym etapie wczytuje si� tylko menu,
oraz wszytkie elementy, kt�re s� przydatne na ka�dym etapie gry.

Zawarto�� klasy IGamePlay powinna by� jak najmniejsza, poniewa� wszystkie te rzeczy s� wczytywane
zanim cokolwiek pojawi si� na ekranie. Z tego wzgl�du lepiej najpierw wczyta� ma�o, �eby u�ytkownik
ju� co� zobaczy�, a potem dopiero wczyta� reszt� wy�wietlaj�c jednocze�nie pasek wczytywania.

Funkcja nie jest dost�pna w EngineInterface. S�u�y do wczytania tylko pierwszej klasy IGamePlay
jaka istnieje, na p�niejszych etapach gry robi si� to innymi funkcjami.

Level si� nie wczyta, je�eli nie zainicjowano DirectXa. Funkcje tworz�ce bufory, textury
i tym podobne rzeczy wymagaj� zainicjowanego kontekstu urz�dzenia DirectX, dlatego 
na wszelki wypadek zawsze inicjalizacja powinna by� wcze�niej.

@param[in] game_play Obiekt do wczytania, jako pocz�tek gry.
@see IGamePlay*/
void Engine::set_entry_point( IGamePlay* game_play )
{
	if ( Context.engineReady )
	{
		game_play->set_engine_and_fable( this, Context.fableEngine );
		Context.fableEngine->set_game_play( game_play );
		
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
		IGamePlay* game_play;


		if ( directX_ready )
		{
			fableEngine->set_game_play( game_play );
			game_play->load_level( );
		}
	}
}
#endif