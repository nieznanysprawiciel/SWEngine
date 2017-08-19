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
	// Dziêki tej zmiennej bêdzie mo¿na wysy³aæ eventy
	ActorBase::SetEngine( this );

	Context->fullScreen = false;			//inicjalizacja jako false potrzebna w funkcji init_window
	Context->engineReady = false;			//jeszcze nie zainicjowaliœmy

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

// ================================ //
//
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


// ================================ //
//
bool		Engine::InitGUIConfiguration()
{
	m_guiConfig.UseBlockingMode = false;
	m_guiConfig.UseVSync = true;
	m_guiConfig.RedrawOnlyFocused = false;

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
void			Engine::SetEntryPoint( IGamePlay* game_play )
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


}	// sw

