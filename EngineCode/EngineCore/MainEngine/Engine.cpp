/**@file Engine.cpp
@author nieznanysprawiciel
@copyright Plik jest cz�ci� silnika graficznego SWEngine.

@brief Plik zawiera definicje metod klasy Engine dotycz�cych inicjacji i zwalniania DirectXa
oraz g��wne funkcje do renderingu.
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
	// Dzi�ki tej zmiennej b�dzie mo�na wysy�a� eventy
	ActorBase::SetEngine( this );

#ifndef __UNUSED
	//Zmienna decyduje o konczeniu w�tk�w
	join_render_thread = false;
#endif

	Context->fullScreen = false;			//inicjalizacja jako false potrzebna w funkcji init_window
	Context->engineReady = false;			//jeszcze nie zainicjowali�my
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
	//obiekty trzeba pokasowa�, zanim si� skasuje to, do czego si� odwo�uj�
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
//								inicjalizacja okna i modu��w zewn�trznych						//
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

///@brief Inicjuje dzia�anie silnika.
///W trybie fullscreen szeroko�� i wysoko�� okna jest ignorowana, a dane s� pobierane z systemu.
///@param[in] width Szeroko�� okna
///@param[in] height Wysoko�� okna
///@param[in] fullscreen Pe�ny ekran lub renderowanie w oknie
///@param[in] nCmdShow Czwarty parametr funkcji WinMain. Decyduje w jaki spos�b powinno zosta� pokazane okno aplikacji.
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

	// Czym p�niej zainicjujemy tym lepiej.
	Context->timeManager.InitTimer();

	Context->engineReady = true;		//jeste�my gotowi do renderowania

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

/**@brief Inicjuje urz�dzenie wej�cia. Domy�lnie u�ywany jest @ref DirectInputModule.*/
bool		Engine::InitInputModule		()
{
	sw::input::IInput* newModule = sw::input::InputFactory::CreateDirectInput();

	sw::input::InputInitInfo info;
	info.AppInstance = Context->instanceHandler;
	info.WndHandle = Context->windowHandler;

	bool result = newModule->Init( info );
	if( result )
	{
		// Normalnie trzeba skasowa� zwracany przez t� funkcj� modu�.
		// W tym przypadku wiemy, �e jest nullptrem, poniewa� dopiero inicjujemy aplikacj�.
		auto lastModule = Context->ui_engine->ChangeInputModule( newModule );
		assert( lastModule == nullptr );	// Na wypadek jakby kto� kiedy� u�y� tej funkcji nie w inicjalizacji.
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

/**@brief W tej funkcji wywo�ywane s� wszystkie funkcje, kt�re aktualizuj� stan silnika
po wykonaniu ca�ej logiki w danej klatce.

Stan silnika jest w czasie trwania klatki niezmienny. Jest tak dlatego, �e logika gry
jest (b�dzie) wykonywana wielow�tkowo i aktorzy odpytuj�cy o stan powinni dosta� informacj�
o tym co by�o w ostaniej klatce, a nie co jest w danym momencie, poniewa� taki tymczasowy
stan w zasadzie niewiele m�wi.*/
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

		//Renderujemy scen� oraz interfejs u�ytkownika
	Context->displayEngine->BeginScene();

	Context->displayEngine->DisplayScene( timeInterval, framePercent );
	Context->ui_engine->DrawGUI( timeInterval, framePercent );

	END_PERFORMANCE_CHECK( RENDERING_TIME )		///< Ze wzgl�du na V-sync test wykonujemy przed wywyo�aniem funkcji present.

	Context->displayEngine->EndScene();
}


/**
@brief Zwraca wska�nik na g��wny render target. Render target jest tworzony w tej funkcji.

Funkcja stworzona dla edytora, kt�ry zamierza wy�wietla� zawarto�� render targetu w swoim oknie.
U�ywa� rozs�dnie.

@return Wska�nik na render target.
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

/**@brief Zwraca struktur� EngineContext.
@attention Dzi�ki temu mo�na zrobi� w zasadzie wszystko i to du�o u�atwia, ale mo�na si� skrzywdzi�.*/
CameraData&		Engine::GetMainCamera()
{
	return Context->controllersEngine->GetGlobalState()->Camera;;
}


#ifndef __UNUSED

/**@brief Funkcja oblicza interwa� czasowy jaki up�yn�� od ostatniej ramki.
 *Poza tym s� tu generowane eventy dotycz�ce czasu, op�nie� itp.
 @param[out] time_interval Zwraca interwa� jaki up�yn�� od ostatniego wywo�ania.*/
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

	//zliczanie FPS�w
	elapsed_time += time_diff;
	if( elapsed_time >= FRAMES_PER_SEC_UPDATE * timer_frequency )	//aktualizujemy co 10 sekund
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

@deprecated Use internal api for that.
@see Events
@param[in] new_event Event do wys�ania.*/
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
void Engine::SetEntryPoint( IGamePlay* game_play )
{
	if( Context->engineReady )
	{
		game_play->SetEngineReference( this, Context->fableEngine );
		Context->fableEngine->SetGamePlay( game_play );

		int result = game_play->LoadLevel();
		if( result )
		{//Tutaj mo�e si� znale�� obs�uga b��d�w

		}
	}
}

#ifndef __UNUSED
///@brief Nie b�dzie wczytywania z bibliotek DLL. Maj� one ddzieln� stert� i powoduje to problemy ze zwalnianiem
///pami�ci. Poza tym taka architektura nie nadaje si� do przeci��ania operator�w new i delete.
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

