/**@file Engine.cpp
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.

@brief Plik zawiera definicje metod klasy Engine dotycz¹cych inicjacji i zwalniania DirectXa
oraz g³ówne funkcje do renderingu.
*/
#include "EngineCore/stdafx.h"
#include "EngineCore/MainEngine/Engine.h"

#include "EngineContext.h"

#include "swGraphicAPI/Resources/ResourcesFactory.h"
#include "EngineCore/GamePlay/IGamePlay.h"
#include "EngineCore/EngineHelpers/PerformanceCheck.h"

#include "swInputLibrary/InputCore/Factory.h"

#include "Initializers/Config.h"

#include "swCommonLib/Serialization/Serializer.h"
#include "swCommonLib/Serialization/PropertySerialization/EngineSerializationContext.h"

#include "swCommonLib/Common/MemoryLeaks.h"



namespace sw
{

//----------------------------------------------------------------------------------------------//
//								konstruktor i destruktor										//
//----------------------------------------------------------------------------------------------//


// ================================ //
//
Engine::Engine( HINSTANCE instanceHandle )
	:	sw::gui::GUISystem( 0, nullptr, nullptr )
{
	InternalInit( instanceHandle );
}

// ================================ //
//
Engine::Engine()
	:	sw::gui::GUISystem( 0, nullptr, nullptr )
{
	InternalInit( GetModuleHandle( nullptr ) );
}


// ================================ //
//
Engine::Engine( int argc, char** argv, sw::gui::INativeGUI* gui )
	:	sw::gui::GUISystem( argc, argv, gui )
{
	InternalInit( GetModuleHandle( nullptr ) );
}

/**@brief Contructor helper. Creates all modules without initialization logic.*/
void	Engine::InternalInit			( HINSTANCE instanceHandle )
{
	// Dziêki tej zmiennej bêdzie mo¿na wysy³aæ eventy
	ActorBase::SetEngine( this );

#ifndef __UNUSED
	//Zmienna decyduje o konczeniu w¹tków
	join_render_thread = false;
#endif

	Context->fullScreen = false;			//inicjalizacja jako false potrzebna w funkcji init_window
	Context->engineReady = false;			//jeszcze nie zainicjowaliœmy
	Context->instanceHandler = instanceHandle;

	Context->config					= new Config( "configs/StartConfig.config" );

	Context->graphicInitializer		= nullptr;			// GUI initializes.

	Context->controllersEngine		= new ControllersEngine( this );
	Context->movementEngine			= new MovementEngine( this );
	Context->displayEngine			= new DisplayEngine( this );
	Context->collisionEngine		= new CollisionEngine( this );
	Context->physicEngine			= new PhysicEngine( this );
	Context->modelsManager			= new AssetsManager( this );
	Context->fableEngine			= new FableEngine( this );
	Context->soundEngine			= new SoundEngine( this );
	Context->ui_engine				= new UIEngine( this );
	Context->actorsManager			= new ActorsManager( this );
	Context->eventsManager			= new EventManager( this );

	// Initialize frames counter.
	Context->pause = false;
}

Engine::~Engine()
{
	//obiekty trzeba pokasowaæ, zanim siê skasuje to, do czego siê odwo³uj¹
	for( unsigned int i = 0; i < Context->objectList.size(); ++i )
		delete Context->objectList[ i ];

	delete Context->eventsManager;
	delete Context->controllersEngine;
	delete Context->movementEngine;
	delete Context->displayEngine;
	delete Context->collisionEngine;
	delete Context->physicEngine;
	delete Context->fableEngine;
	delete Context->soundEngine;
	delete Context->ui_engine;
	delete Context->actorsManager;
	//delete Context->modelsManager;		// Will be deleted by GUI.

	DefaultAssets::Release();
	//Context->graphicInitializer->ReleaseAPI();
	//delete Context->graphicInitializer;

	delete Context->config;
}


//----------------------------------------------------------------------------------------------//
//								inicjalizacja okna i modu³ów zewnêtrznych						//
//----------------------------------------------------------------------------------------------//

/**@brief Initializes engine.

Uses config to initialize window.*/
bool		Engine::InitEngine				( int nCmdShow )
{
	auto width = Context->config->ScreenWidth();
	auto height = Context->config->ScreenHeight();
	bool fullscreen = Context->config->Fullscreen();

	return TRUE == InitEngine( width, height, fullscreen, nCmdShow );
}

///@brief Inicjuje dzia³anie silnika.
///W trybie fullscreen szerokoœæ i wysokoœæ okna jest ignorowana, a dane s¹ pobierane z systemu.
///@param[in] width Szerokoœæ okna
///@param[in] height Wysokoœæ okna
///@param[in] fullscreen Pe³ny ekran lub renderowanie w oknie
///@param[in] nCmdShow Czwarty parametr funkcji WinMain. Decyduje w jaki sposób powinno zostaæ pokazane okno aplikacji.
int			Engine::InitEngine( int width, int height, bool fullScreen, int nCmdShow )
{
	int result;

	//Tworzenie okna aplikacji
	result = InitWindow( width, height, fullScreen, nCmdShow );
	if( !result )
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

	result = InitDefaultActorsClasses();
	assert( result != 0 );
	if( result == 0 )
		return FALSE;

	// Czym póŸniej zainicjujemy tym lepiej.
	Context->timeManager.InitTimer();

	Context->engineReady = true;		//jesteœmy gotowi do renderowania

	return TRUE;
}

// ================================ //
//
bool		Engine::InitGUIConfiguration()
{
	m_guiConfig.UseBlockingMode = false;
	m_guiConfig.UseVSync = true;

	Context->windowWidth = m_windows[ 0 ]->GetNativeWindow()->GetClientWidth();
	Context->windowHeight = m_windows[ 0 ]->GetNativeWindow()->GetClientHeight();

	return true;
}

/**@brief Engine side graphicAPI initialization.
GraphicAPI was created by GUI before. Now we only initialize engine modules with it.*/
bool		Engine::InitEngineGraphicAPI()
{
	Context->graphicInitializer = m_graphicApi;

	// If there's something to initialize, place it here.
	return true;
}

// ================================ //
//
bool		Engine::InitEngineInputModule()
{
	// Normally we must delete lastModule. In this case we know that it is nullptr.
	auto lastModule = Context->ui_engine->ChangeInputModule( m_input );
	assert( lastModule == nullptr );		// Check to be sure.

	return true;
}

/**@brief Inicjuje urz¹dzenie wejœcia. Domyœlnie u¿ywany jest @ref DirectInputModule.*/
bool		Engine::InitInputModule		()
{
	sw::input::IInput* newModule = sw::input::InputFactory::CreateDirectInput();

	sw::input::InputInitInfo info;
	info.AppInstance = Context->instanceHandler;
	info.WndHandle = Context->windowHandler;

	bool result = newModule->Init( info );
	if( result )
	{
		// Normalnie trzeba skasowaæ zwracany przez tê funkcjê modu³.
		// W tym przypadku wiemy, ¿e jest nullptrem, poniewa¿ dopiero inicjujemy aplikacjê.
		auto lastModule = Context->ui_engine->ChangeInputModule( newModule );
		assert( lastModule == nullptr );	// Na wypadek jakby ktoœ kiedyœ u¿y³ tej funkcji nie w inicjalizacji.
	}
	return result;
}

/**@brief */
bool		Engine::InitSoundModule		()
{
	return true;
}


//----------------------------------------------------------------------------------------------//
//								Object and rendering pipeline									//
//----------------------------------------------------------------------------------------------//

/**@brief Main function called per frame.*/
void		Engine::RenderFrame()
{
	float timeInterval = Context->timeManager.onStartRenderFrame();
	float lag = Context->timeManager.GetFrameLag();

	UpdateScene( lag, timeInterval );
	RenderScene( lag, timeInterval );
}

/**@brief W tej funkcji wywo³ywane s¹ wszystkie funkcje, które aktualizuj¹ stan silnika
po wykonaniu ca³ej logiki w danej klatce.

Stan silnika jest w czasie trwania klatki niezmienny. Jest tak dlatego, ¿e logika gry
jest (bêdzie) wykonywana wielow¹tkowo i aktorzy odpytuj¹cy o stan powinni dostaæ informacjê
o tym co by³o w ostaniej klatce, a nie co jest w danym momencie, poniewa¿ taki tymczasowy
stan w zasadzie niewiele mówi.*/
void		Engine::SingleThreadedUpdatePhase( float& lag, float timeInterval )
{
	Context->controllersEngine->SingleThreadedUpdatePhase( timeInterval );
}


/**@brief Updates input, physics, AI, events and alll connected to game logic.*/
void		Engine::UpdateScene( float& lag, float timeInterval )
{
	// Process input only once. Since we don't call native loop, there shouldn't be any new events to process.
	Context->ui_engine->ProceedInput( FIXED_MOVE_UPDATE_INTERVAL );

	while( lag >= FIXED_MOVE_UPDATE_INTERVAL )
	{
		START_PERFORMANCE_CHECK( FRAME_COMPUTING_TIME )

		Context->physicEngine->ProceedPhysic( FIXED_MOVE_UPDATE_INTERVAL );
		Context->controllersEngine->ProceedControllersPre( FIXED_MOVE_UPDATE_INTERVAL );
		Context->movementEngine->ProceedMovement( FIXED_MOVE_UPDATE_INTERVAL );
		Context->controllersEngine->ProceedControllersPost( FIXED_MOVE_UPDATE_INTERVAL );
		Context->collisionEngine->ProceedCollisions( FIXED_MOVE_UPDATE_INTERVAL );
		Context->fableEngine->ProceedFable( FIXED_MOVE_UPDATE_INTERVAL );
		Context->eventsManager->ProcessEvents( FIXED_MOVE_UPDATE_INTERVAL );

		SingleThreadedUpdatePhase( lag, timeInterval );

		lag -= FIXED_MOVE_UPDATE_INTERVAL;
		Context->timeManager.UpdateTimeLag( lag );

		timeInterval = Context->timeManager.onStartRenderFrame();
		lag = Context->timeManager.GetFrameLag();

		END_PERFORMANCE_CHECK( FRAME_COMPUTING_TIME )
	}
}


/**@brief Renders scene and interpolates objects.*/
void		Engine::RenderScene( float lag, float timeInterval )
{
	float framePercent = lag / FIXED_MOVE_UPDATE_INTERVAL;

#ifdef _INTERPOLATE_POSITIONS
	START_PERFORMANCE_CHECK( INTERPOLATION_TIME )

	Context->displayEngine->InterpolatePositions( framePercent );

	END_PERFORMANCE_CHECK( INTERPOLATION_TIME )
#endif

	START_PERFORMANCE_CHECK( RENDERING_TIME )

		//Renderujemy scenê oraz interfejs u¿ytkownika
	Context->displayEngine->BeginScene();

	Context->displayEngine->DisplayScene( timeInterval, framePercent );
	Context->ui_engine->DrawGUI( timeInterval, framePercent );

	END_PERFORMANCE_CHECK( RENDERING_TIME )		///< Ze wzglêdu na V-sync test wykonujemy przed wywyo³aniem funkcji present.

	Context->displayEngine->EndScene();
}


/**
@brief Zwraca wskaŸnik na g³ówny render target. Render target jest tworzony w tej funkcji.

Funkcja stworzona dla edytora, który zamierza wyœwietlaæ zawartoœæ render targetu w swoim oknie.
U¿ywaæ rozs¹dnie.

@return WskaŸnik na render target.
*/
void*		Engine::GetRenderTargetHandle( uint16 width, uint16 height )
{
	RenderTargetDescriptor descriptor;
	descriptor.AllowShareResource = 1;
	descriptor.TextureWidth = width;
	descriptor.TextureHeight = height;
	descriptor.ColorBuffFormat = ResourceFormat::RESOURCE_FORMAT_B8G8R8A8_UNORM;
	descriptor.TextureType = TextureType::TEXTURE_TYPE_TEXTURE2D;
	descriptor.DepthStencilFormat = DepthStencilFormat::DEPTH_STENCIL_FORMAT_D24_UNORM_S8_UINT;
	descriptor.Usage = ResourceUsage::RESOURCE_USAGE_DEFAULT;

	RenderTargetObject* renderTarget = Context->modelsManager->CreateRenderTarget( DefaultAssets::EDITOR_RENDERTARGET_STRING, descriptor );
	Context->displayEngine->SetMainRenderTarget( renderTarget );

	Context->windowHeight = height;
	Context->windowWidth = width;

	return Context->graphicInitializer->GetRenderTargetHandle( renderTarget );
}

/**@brief Zwraca strukturê EngineContext.
@attention Dziêki temu mo¿na zrobiæ w zasadzie wszystko i to du¿o u³atwia, ale mo¿na siê skrzywdziæ.*/
CameraData&		Engine::GetMainCamera()
{
	return Context->controllersEngine->GetGlobalState()->Camera;;
}


#ifndef __UNUSED

/**@brief Funkcja oblicza interwa³ czasowy jaki up³yn¹³ od ostatniej ramki.
 *Poza tym s¹ tu generowane eventy dotycz¹ce czasu, opóŸnieñ itp.
 @param[out] time_interval Zwraca interwa³ jaki up³yn¹³ od ostatniego wywo³ania.*/
void Engine::time_controller( float& time_interval )
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
	if( elapsed_time >= FRAMES_PER_SEC_UPDATE * timer_frequency )	//aktualizujemy co 10 sekund
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

@deprecated Use internal api for that.
@see Events
@param[in] new_event Event do wys³ania.*/
void			Engine::SendEvent( Event* newEvent )
{
	Context->eventsManager->SendEvent( newEvent );
}


AppInstanceHandle		Engine::GetInstanceHandler	()				{ return Context->instanceHandler; }
WindowHandle			Engine::GetWindowHandler	()				{ return Context->windowHandler; }

int						Engine::GetWindowWidth		()				{ return Context->windowWidth; }
int						Engine::GetWindowHeight		()				{ return Context->windowHeight; }
void					Engine::EnableInput			( bool val )	{ Context->ui_engine->EnableInput( val ); }

input::IInput*			Engine::ChangeInputModule	( input::IInput* newModule )	{ return Context->ui_engine->ChangeInputModule( newModule ); }

AssetsManager *			Engine::GetAssetsManager	()				{ return Context->modelsManager; }




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
void Engine::SetEntryPoint( IGamePlay* game_play )
{
	if( Context->engineReady )
	{
		game_play->SetEngineReference( this, Context->fableEngine );
		Context->fableEngine->SetGamePlay( game_play );

		int result = game_play->LoadLevel();
		if( result )
		{//Tutaj mo¿e siê znaleŸæ obs³uga b³êdów

		}
	}
}

#ifndef __UNUSED
///@brief Nie bêdzie wczytywania z bibliotek DLL. Maj¹ one ddzieln¹ stertê i powoduje to problemy ze zwalnianiem
///pamiêci. Poza tym taka architektura nie nadaje siê do przeci¹¿ania operatorów new i delete.
void Engine::SetEntryPoint( const std::wstring dll_name )
{
	HINSTANCE dll_entry_point;
	dll_entry_point = LoadLibrary( dll_name.c_str() );

	if( dll_entry_point != NULL )
	{
		IGamePlay* game_play;


		if( directX_ready )
		{
			fableEngine->set_game_play( game_play );
			game_play->LoadLevel();
		}
}
}
#endif

}	// sw

