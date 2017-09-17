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
Engine::Engine( AppInstanceHandle instanceHandle )
	:	sw::gui::GUISystem( 0, nullptr, nullptr )
{
	InternalInit();
}

// ================================ //
//
Engine::Engine()
	:	sw::gui::GUISystem( 0, nullptr, nullptr )
{
	InternalInit();
}


// ================================ //
//
Engine::Engine( int argc, char** argv, sw::gui::INativeGUI* gui )
	:	sw::gui::GUISystem( argc, argv, gui )
{
	InternalInit();
}

/**@brief Contructor helper. Creates all modules without initialization logic.*/
void	Engine::InternalInit			()
{
	// Dzi�ki tej zmiennej b�dzie mo�na wysy�a� eventy
	ActorBase::SetEngine( this );

	Context->engineReady = false;			//jeszcze nie zainicjowali�my

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

	// Editor/Engine mode initialization variables.
	m_showWindowOnStart = true;
}

// ================================ //
//
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

// ================================ //
//
void			Engine::StartEditorMode				()
{
	m_showWindowOnStart = false;
}


//----------------------------------------------------------------------------------------------//
//								inicjalizacja okna i modu��w zewn�trznych						//
//----------------------------------------------------------------------------------------------//


// ================================ //
//
bool			Engine::Initialize					()
{
	bool result = true;
	bool debug = false;

	// In future move this as option to config or somewhere.
#ifdef _DEBUG
	debug = true;
#endif

	result = result && InitResourceManager( Context->modelsManager );

	result = result && DefaultInitGraphicAPI( debug, false );
	result = result && DefaultInitNativeGUI();
	result = result && DefaultInitRenderingSystem();

	result = result && DefaultInitFirstWindow( Context->config->ScreenWidth(), Context->config->ScreenHeight(), "Sleeping Wombat Engine (DirectX 11)", m_showWindowOnStart );

	return result;
}

// ================================ //
//
bool			Engine::OnInitialized				()
{
	// Here goes all modules initialization.
	bool result = true;

	result = result && InitGUIConfiguration();
	result = result && InitEngineGraphicAPI();
	result = result && InitEngineInputModule();
	result = result && InitSoundModule();
	result = result && InitDefaultAssets();
	result = result && InitDisplayer();
	result = result && InitDefaultActorsClasses();

	// It's better to initialize timer as late as posible.
	Context->timeManager.InitTimer();

	Context->engineReady = result;
	return result;
}

// ================================ //
//
void			Engine::OnClosing					()
{}

// ================================ //
//
void			Engine::OnIdle						( const gui::FrameTime& frameTime )
{
	RenderFrame( frameTime.Time, frameTime.Elapsed );
}




// ================================ //
//
bool		Engine::InitGUIConfiguration			()
{
	m_guiConfig.UseBlockingMode = false;
	m_guiConfig.UseVSync = true;
	m_guiConfig.RedrawOnlyFocused = false;

	Context->windowWidth = m_windows[ 0 ]->GetNativeWindow()->GetClientWidth();
	Context->windowHeight = m_windows[ 0 ]->GetNativeWindow()->GetClientHeight();

	m_clock.SetFixedStep( (TimeDiff)FIXED_MOVE_UPDATE_INTERVAL );

	return true;
}

/**@brief Engine side graphicAPI initialization.
GraphicAPI was created by GUI before. Now we only initialize engine modules with it.*/
bool		Engine::InitEngineGraphicAPI			()
{
	Context->graphicInitializer = m_graphicApi;

	// If there's something to initialize, place it here.
	return true;
}

// ================================ //
//
bool		Engine::InitEngineInputModule			()
{
	// Normally we must delete lastModule. In this case we know that it is nullptr.
	auto lastModule = Context->ui_engine->ChangeInputModule( m_input );
	assert( lastModule == nullptr );		// Check to be sure.

	return true;
}

/**@brief */
bool		Engine::InitSoundModule					()
{
	return true;
}


//----------------------------------------------------------------------------------------------//
//								Object and rendering pipeline									//
//----------------------------------------------------------------------------------------------//

/**@brief Main function called per frame.*/
void		Engine::RenderFrame						( TimeType time, TimeDiff elapsed )
{
	float timeInterval = Context->timeManager.onStartRenderFrame();
	float lag = Context->timeManager.GetFrameLag();

	UpdateScene( time, elapsed );
	RenderScene( time, elapsed );
}

/**@brief W tej funkcji wywo�ywane s� wszystkie funkcje, kt�re aktualizuj� stan silnika
po wykonaniu ca�ej logiki w danej klatce.

Stan silnika jest w czasie trwania klatki niezmienny. Jest tak dlatego, �e logika gry
jest (b�dzie) wykonywana wielow�tkowo i aktorzy odpytuj�cy o stan powinni dosta� informacj�
o tym co by�o w ostaniej klatce, a nie co jest w danym momencie, poniewa� taki tymczasowy
stan w zasadzie niewiele m�wi.*/
void		Engine::SingleThreadedUpdatePhase		( TimeType time, TimeDiff elapsed )
{
	Context->controllersEngine->SingleThreadedUpdatePhase( time, elapsed );
}


/**@brief Updates input, physics, AI, events and alll connected to game logic.*/
void		Engine::UpdateScene						( TimeType time, TimeDiff elapsed )
{
	TimeType frameTime = time - elapsed + FIXED_MOVE_UPDATE_INTERVAL;
	TimeDiff frameElapsed = elapsed;

	// Process input only once. Since we don't call native loop, there shouldn't be any new events to process.
	Context->ui_engine->ProceedInput( time, frameElapsed );

	// If to much time elapsed, we must process a few frames at once. This way logic updates are independent from frame rate.
	while( frameElapsed >= FIXED_MOVE_UPDATE_INTERVAL )
	{
		START_PERFORMANCE_CHECK( FRAME_COMPUTING_TIME )

		Context->physicEngine->ProceedPhysic					( frameTime, FIXED_MOVE_UPDATE_INTERVAL );
		Context->controllersEngine->ProceedControllersPre		( frameTime, FIXED_MOVE_UPDATE_INTERVAL );
		Context->movementEngine->ProceedMovement				( frameTime, FIXED_MOVE_UPDATE_INTERVAL );
		Context->controllersEngine->ProceedControllersPost		( frameTime, FIXED_MOVE_UPDATE_INTERVAL );
		Context->collisionEngine->ProceedCollisions				( frameTime, FIXED_MOVE_UPDATE_INTERVAL );
		Context->fableEngine->ProceedFable						( frameTime, FIXED_MOVE_UPDATE_INTERVAL );
		Context->eventsManager->ProcessEvents					( frameTime, FIXED_MOVE_UPDATE_INTERVAL );

		SingleThreadedUpdatePhase								( frameTime, FIXED_MOVE_UPDATE_INTERVAL );

		END_PERFORMANCE_CHECK( FRAME_COMPUTING_TIME )

		frameElapsed -= FIXED_MOVE_UPDATE_INTERVAL;
		frameTime += FIXED_MOVE_UPDATE_INTERVAL;
	}
}


/**@brief Renders scene and interpolates objects.*/
void		Engine::RenderScene						( TimeType time, TimeDiff elapsed )
{
	float framePercent = (float)m_clock.GetFrameFraction();

#ifdef _INTERPOLATE_POSITIONS
	START_PERFORMANCE_CHECK( INTERPOLATION_TIME )

	Context->displayEngine->InterpolatePositions( framePercent );

	END_PERFORMANCE_CHECK( INTERPOLATION_TIME )
#endif

	START_PERFORMANCE_CHECK( RENDERING_TIME )

		//Renderujemy scen� oraz interfejs u�ytkownika
	Context->displayEngine->BeginScene();

	Context->displayEngine->DisplayScene( time, elapsed, framePercent );
	Context->ui_engine->DrawGUI( time, elapsed );

	END_PERFORMANCE_CHECK( RENDERING_TIME )		///< Ze wzgl�du na V-sync test wykonujemy przed wywyo�aniem funkcji present.

	Context->displayEngine->EndScene();
}



//----------------------------------------------------------------------------------------------//
//								funkcje pomocnicze												//
//----------------------------------------------------------------------------------------------//


/**@brief Funkcja pozwala wys�a� event, kt�ry b�dzie potem przetworzony przez klase FableEngine.

@deprecated Use internal api for that.
@see Events
@param[in] new_event Event do wys�ania.*/
void			Engine::SendEvent					( Event* newEvent )
{
	Context->eventsManager->SendEvent( newEvent );
}


int						Engine::GetWindowWidth		()				{ return Context->windowWidth; }
int						Engine::GetWindowHeight		()				{ return Context->windowHeight; }

AssetsManager *			Engine::GetAssetsManager	()				{ return Context->modelsManager; }



/**@brief Requests native gui to end application.*/
void		Engine::EndAplication()
{
	m_nativeGUI->EndApplication();
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
void			Engine::SetEntryPoint( IGamePlay* game_play )
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


}	// sw


/**
@page MainLoop P�tla g��wna silnika

@section Contents Spis Tre�ci
- @ref Code
- @ref Description


@subsection Code Kod
@code
int Engine::MainLoop()
{
	MSG msg;

#ifndef __UNUSED
	//w��czamy g��wny w�tek do renderingu
	std::thread main_thread(main_thread_function, this);
#endif

	// Main message loop:
	while (TRUE)
	{
		if ( engineReady )
			RenderFrame();

		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		//while (GetMessage(&msg, NULL, 0, 0))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (msg.message == WM_QUIT)
		{
#ifndef __UNUSED
			join_render_thread = true;	//to jest jedyne miejsce, w kt�rym ta zmienna mo�e zostac ustawiona
										//dlatego mo�emy to zrobi� bez synchronizacji
			main_thread.join();
#endif
			break;
		}
	}

	return (int)msg.wParam;
}
@endcode
@subsection Description Opis
P�tla g��wna silnika zast�puje Windowsowsk� p�tl� komunikat�w, wywo�uj�c jednocze�nie p�tl� g��wn� silnika (zobacz temat: Potok przetwarzania obiekt�w).
Musimy obs�ugiwa� komunikaty windowsa, poniewa� w przeciwnym razie m�g�by on uzna�, �e nasz program si� zawiesi�, i spr�bowa� zabi� nasz proces.
Dlatego pobieramy wiadomo�ci z kolejki komunikat�w i przekazujemy do dalszego przetwarzania.
Do pobierania komunikat�w u�ywamy nieblokuj�cej funkcji PeekMessage (nie GetMessage).

Jako p�tla g��wna silnika s�u�y funkcja render_frame. Jest ona wywo�ywana tylko pod warunkiem, �e wcze�niejsza inicjalizacja
directXa w pe�ni powiod�a si�. Wa�nym punktem jest inicjacja zmiennej time_previous. Niezainicjowanie tej zmiennej, mog�oby
spowodowa�, �e w pierwszej klatce nast�pi�aby nieprzewidywalna zmiana po�o�e� i orientacji wszystkich obiekt�w na scenie.

Wyj�cie z aplikacji nast�puje kiedy w kolejce komunikat�w windowsa znajdzie si� komunikat WM_QUIT. Z wn�trza silnika mo�na to osi�gn�� przez wywo�anie funkcji Engine::EndAplication.

Definicja: "Engine.cpp"
*/


