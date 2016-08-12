/**@file Engine.cpp
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.

@brief Plik zawiera definicje metod klasy Engine dotycz¹cych inicjacji i zwalniania DirectXa
oraz g³ówne funkcje do renderingu.
*/
#include "EngineCore/stdafx.h"
#include "EngineCore/MainEngine/Engine.h"

#include "EngineContext.h"

#include "GraphicAPI/ResourcesFactory.h"
#include "EngineCore/GamePlay/IGamePlay.h"
#include "EngineCore/EngineHelpers/PerformanceCheck.h"

#include "InputLibrary/Factory.h"

#include "Common/Serialization/Serializer.h"
#include "Common/Serialization/SW/EngineSerializationContext.h"

#include "Common/MemoryLeaks.h"




REGISTER_PERFORMANCE_CHECK( RENDERING_TIME )
REGISTER_PERFORMANCE_CHECK( FRAME_COMPUTING_TIME )
#ifdef _INTERPOLATE_POSITIONS
	REGISTER_PERFORMANCE_CHECK( INTERPOLATION_TIME )
#endif


//----------------------------------------------------------------------------------------------//
//								konstruktor i destruktor										//
//----------------------------------------------------------------------------------------------//


Engine::Engine(HINSTANCE instance)
{
	// Dziêki tej zmiennej bêdzie mo¿na wysy³aæ eventy
	ActorBase::SetEngine( this );

#ifndef __UNUSED
	//Zmienna decyduje o konczeniu w¹tków
	join_render_thread = false;
#endif

	Context->fullScreen = false;			//inicjalizacja jako false potrzebna w funkcji init_window
	Context->engineReady = false;			//jeszcze nie zainicjowaliœmy
	Context->instanceHandler = instance;

	// Initialize global strings
	LoadString(Context->instanceHandler, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(Context->instanceHandler, IDC_SW_ENGINE, szWindowClass, MAX_LOADSTRING);

	Context->graphicInitializer = ResourcesFactory::CreateAPIInitializer();

	Context->controllersEngine		= new ControllersEngine( this );
	Context->movementEngine			= new MovementEngine( this );
	Context->displayEngine			= new DisplayEngine( this );
	Context->collisionEngine		= new CollisionEngine( this );
	Context->physicEngine			= new PhysicEngine( this );
	Context->modelsManager			= new ModelsManager( this );
	Context->fableEngine			= new FableEngine( this );
	Context->soundEngine			= new SoundEngine( this );
	Context->ui_engine				= new UI_Engine( this );
	Context->actorsManager			= new ActorsManager( this );
	Context->eventsManager			= new EventManager( this );

	//inicjujemy licznik klatek
	Context->pause = false;
}


Engine::~Engine()
{
	//obiekty trzeba pokasowaæ, zanim siê skasuje to, do czego siê odwo³uj¹
	for ( unsigned int i = 0; i < Context->objectList.size(); ++i )
		delete Context->objectList[i];

	delete Context->controllersEngine;
	delete Context->movementEngine;
	delete Context->displayEngine;
	delete Context->collisionEngine;
	delete Context->physicEngine;
	delete Context->fableEngine;
	delete Context->soundEngine;
	delete Context->ui_engine;			//sprz¹ta po directinpucie
	delete Context->actorsManager;
	delete Context->modelsManager;		//musi byæ kasowany na koñcu

	DefaultAssets::Release();
	Context->graphicInitializer->ReleaseAPI();
	delete Context->graphicInitializer;
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

	result = InitDefaultActorsClasses();
	assert( result != 0 );
	if( result == 0 )
		return FALSE;

	// Czym póŸniej zainicjujemy tym lepiej.
	Context->timeManager.InitTimer();

	Context->engineReady = true;		//jesteœmy gotowi do renderowania

	return TRUE;
}


/**@brief */
bool Engine::InitGraphicAPI( int width, int height, bool fullScreen )
{
	bool result;

	//Inicjalizowanie API graficznego
	GraphicAPIInitData initData;
	initData.SwapChain.FullScreen			= fullScreen;
	initData.SingleThreaded					= false;
	initData.SwapChain.WindowHandle			= (uint32)Context->windowHandler;
	initData.SwapChain.WindowHeight			= height;
	initData.SwapChain.WindowWidth			= width;
	initData.SwapChain.DepthStencilFormat	= ResourceFormat::RESOURCE_FORMAT_D24_UNORM_S8_UINT;
	result = Context->graphicInitializer->InitAPI( initData );
	assert( result != 0 );
	if( result == 0 )
		return false;
	return true;
}

/**@brief Inicjuje urz¹dzenie wejœcia. Domyœlnie u¿ywany jest @ref DirectInputModule.*/
bool Engine::InitInputModule		()
{
	IInput* newModule = InputFactory::CreateDirectInput();

	InputInitInfo info;
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
bool Engine::InitSoundModule		()
{
	return true;
}

//----------------------------------------------------------------------------------------------//
//								Przygotowanie modu³ów do dzia³ania								//
//----------------------------------------------------------------------------------------------//




//----------------------------------------------------------------------------------------------//
//								potok przetwarzania obiektów									//
//----------------------------------------------------------------------------------------------//

/**@brief To tutaj dziej¹ siê wszystkie rzeczy, które s¹ wywo³ywane co ka¿d¹ klatkê.*/
void Engine::RenderFrame()
{
	float timeInterval = Context->timeManager.onStartRenderFrame();
	float lag = Context->timeManager.GetFrameLag();


	while ( lag >= FIXED_MOVE_UPDATE_INTERVAL )
	{
		START_PERFORMANCE_CHECK(FRAME_COMPUTING_TIME)

		Context->ui_engine->ProceedInput( FIXED_MOVE_UPDATE_INTERVAL );
		Context->physicEngine->ProceedPhysic( FIXED_MOVE_UPDATE_INTERVAL );
		Context->controllersEngine->ProceedControllersPre( FIXED_MOVE_UPDATE_INTERVAL );
		Context->movementEngine->ProceedMovement( FIXED_MOVE_UPDATE_INTERVAL );
		Context->controllersEngine->ProceedControllersPost( FIXED_MOVE_UPDATE_INTERVAL );
		Context->collisionEngine->ProceedCollisions( FIXED_MOVE_UPDATE_INTERVAL );
		Context->fableEngine->ProceedFable( FIXED_MOVE_UPDATE_INTERVAL );
		Context->eventsManager->ProcessEvents( FIXED_MOVE_UPDATE_INTERVAL );

		SingleThreadedUpdatePhase( lag, timeInterval );

		// Aktualizacja czasu po wykonaniu kodu dla danej klatki.
		lag -= FIXED_MOVE_UPDATE_INTERVAL;
		Context->timeManager.UpdateTimeLag( lag );

		timeInterval = Context->timeManager.onStartRenderFrame();
		lag = Context->timeManager.GetFrameLag();

		END_PERFORMANCE_CHECK( FRAME_COMPUTING_TIME )
	}


#ifdef _INTERPOLATE_POSITIONS
	START_PERFORMANCE_CHECK( INTERPOLATION_TIME )

	Context->displayEngine->InterpolatePositions( lag / FIXED_MOVE_UPDATE_INTERVAL );

	END_PERFORMANCE_CHECK( INTERPOLATION_TIME )
#endif

	START_PERFORMANCE_CHECK( RENDERING_TIME )

	//Renderujemy scenê oraz interfejs u¿ytkownika
	Context->displayEngine->BeginScene();

	Context->displayEngine->DisplayScene( timeInterval, lag / FIXED_MOVE_UPDATE_INTERVAL );
	Context->ui_engine->DrawGUI( timeInterval, lag / FIXED_MOVE_UPDATE_INTERVAL );

	END_PERFORMANCE_CHECK( RENDERING_TIME )		///< Ze wzglêdu na V-sync test wykonujemy przed wywyo³aniem funkcji present.

	Context->displayEngine->EndScene();
}

/**@brief W tej funkcji wywo³ywane s¹ wszystkie funkcje, które aktualizuj¹ stan silnika
po wykonaniu ca³ej logiki w danej klatce.

Stan silnika jest w czasie trwania klatki niezmienny. Jest tak dlatego, ¿e logika gry
jest (bêdzie) wykonywana wielow¹tkowo i aktorzy odpytuj¹cy o stan powinni dostaæ informacjê
o tym co by³o w ostaniej klatce, a nie co jest w danym momencie, poniewa¿ taki tymczasowy
stan w zasadzie niewiele mówi.*/
void Engine::SingleThreadedUpdatePhase( float& lag, float timeInterval )
{
	Context->controllersEngine->SingleThreadedUpdatePhase( timeInterval );
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

		Context->ui_engine->ProceedInput( FIXED_MOVE_UPDATE_INTERVAL );
		Context->physicEngine->ProceedPhysic( FIXED_MOVE_UPDATE_INTERVAL );
		Context->controllersEngine->ProceedControllersPre( FIXED_MOVE_UPDATE_INTERVAL );
		Context->movementEngine->ProceedMovement( FIXED_MOVE_UPDATE_INTERVAL );
		Context->controllersEngine->ProceedControllersPost( FIXED_MOVE_UPDATE_INTERVAL );
		Context->collisionEngine->ProceedCollisions( FIXED_MOVE_UPDATE_INTERVAL );
		Context->fableEngine->ProceedFable( FIXED_MOVE_UPDATE_INTERVAL );
		Context->eventsManager->ProcessEvents( FIXED_MOVE_UPDATE_INTERVAL );

		SingleThreadedUpdatePhase( lag, timeInterval );

		lag -= FIXED_MOVE_UPDATE_INTERVAL;
		//timeManager.UpdateTimeLag( lag );

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
void* Engine::GetRenderTargetHandle( uint16 width, uint16 height )
{
	RenderTargetDescriptor descriptor;
	descriptor.AllowShareResource = 1;
	descriptor.TextureWidth = width;
	descriptor.TextureHeight = height;
	descriptor.colorBuffFormat = ResourceFormat::RESOURCE_FORMAT_B8G8R8A8_UNORM;
	descriptor.TextureType = TextureType::TEXTURE_TYPE_TEXTURE2D;
	descriptor.depthStencilFormat = DepthStencilFormat::DEPTH_STENCIL_FORMAT_D24_UNORM_S8_UINT;
	descriptor.Usage = ResourceUsage::RESOURCE_USAGE_DEFAULT;

	RenderTargetObject* renderTarget = Context->modelsManager->CreateRenderTarget( EDITOR_RENDERTARGET_STRING, descriptor );
	Context->displayEngine->SetMainRenderTarget( renderTarget );
	Context->displayEngine->SetProjectionMatrix( DirectX::XMConvertToRadians( 45 ), (float)width / (float)height, 1, 100000 );

	Context->windowHeight = height;
	Context->windowWidth = width;

	return Context->graphicInitializer->GetRenderTargetHandle( renderTarget );
}

/**@brief Zwraca strukturê EngineContext.
@attention Dziêki temu mo¿na zrobiæ w zasadzie wszystko i to du¿o u³atwia, ale mo¿na siê skrzywdziæ.*/
CameraData& Engine::GetMainCamera()
{
	return Context->controllersEngine->GetGlobalState()->Camera;;
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
void			Engine::SendEvent( Event* newEvent )
{
	Context->eventsManager->SendEvent( newEvent );
}


HINSTANCE		Engine::GetInstanceHandler()								{ return Context->instanceHandler; }
HWND			Engine::GetWindowHandler()									{ return Context->windowHandler; }

int				Engine::GetWindowWidth()									{ return Context->windowWidth; }
int				Engine::GetWindowHeight()									{ return Context->windowHeight; }
void			Engine::EnableInput( bool val )								{ Context->ui_engine->EnableInput( val ); }

IInput*			Engine::ChangeInputModule( IInput* newModule )				{ return Context->ui_engine->ChangeInputModule( newModule ); }

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
	if ( Context->engineReady )
	{
		game_play->SetEngineReference( this, Context->fableEngine );
		Context->fableEngine->SetGamePlay( game_play );
		
		int result = game_play->LoadLevel();
		if ( result )
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

	if ( dll_entry_point != NULL )
	{
		IGamePlay* game_play;


		if ( directX_ready )
		{
			fableEngine->set_game_play( game_play );
			game_play->LoadLevel( );
		}
	}
}
#endif