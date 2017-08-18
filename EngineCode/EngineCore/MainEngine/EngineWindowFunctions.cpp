/**@file EngineWindowFunctions.cpp
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.

@brief Plik zawiera definicje metod klasy Engine dotycz¹ce tworzenia okna, obs³ugi pêtli komunikatów
WinAPI oraz g³ówn¹ pêtlê programu @ref Engine::MainLoop.
*/

#include "EngineCore/stdafx.h"
#include "Engine.h"
#include "EngineCore/MainEngine/EngineContext.h"
#include "EngineCore/MainEngine/Initializers/Config.h"

#include "EngineCore/SW_engine.h"


#include <thread>



#include "swCommonLib/Common/MemoryLeaks.h"



//global variables
LPCWSTR szTitle = L"SWEngine (DirectX 11)";					// The title bar text
LPCWSTR szWindowClass = L"SWEnigne window";			// the main window class name

LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);



namespace sw
{


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

	result = result && DefaultInitFirstWindow( Context->config->ScreenWidth(), Context->config->ScreenHeight(), "Sleeping Wombat Engine (DirectX 11)", true );

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
void			Engine::OnIdle						()
{
	RenderFrame();
}




void main_thread_function( Engine* engine );


//----------------------------------------------------------------------------------------------//
//								window functions												//
//----------------------------------------------------------------------------------------------//

/**@brief Rejestruje klasê okna aplikacji.*/
ATOM Engine::EngineRegisterClass()
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof( WNDCLASSEX );

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = (HINSTANCE)Context->instanceHandler;
	wcex.hIcon = LoadIcon( (HINSTANCE)Context->instanceHandler, MAKEINTRESOURCE( IDI_SW_ENGINE ) );
	wcex.hCursor = LoadCursor( NULL, IDC_ARROW );
	wcex.hbrBackground = (HBRUSH)( COLOR_WINDOW + 1 );
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon( wcex.hInstance, MAKEINTRESOURCE( IDI_SW_ENGINE ) );

	return RegisterClassEx( &wcex );
}


/**@brief Inicjuje okno aplikacji.

Funkcja inicjuje i tworzy okno.
Okno nie jest pokazywane na ekranie. Do tego trzeba u¿yæ funkcji Engine::ShowAppWindow.

@return Zwraca TRUE, je¿eli inicjowanie okna powiod³o siê.*/
BOOL Engine::InitInstance( int nCmdShow )
{
	RECT windowRect ={ 0, 0, Context->windowWidth, Context->windowHeight };
	AdjustWindowRect( &windowRect, WS_OVERLAPPEDWINDOW^WS_THICKFRAME, FALSE );

	if( Context->fullScreen )
		Context->windowHandler = CreateWindowEx( NULL, szWindowClass, szTitle, WS_EX_TOPMOST | WS_POPUP,
												 0, 0, Context->windowWidth, Context->windowHeight,
												 NULL, NULL, (HINSTANCE)Context->instanceHandler, NULL );
	else
		Context->windowHandler = CreateWindowEx( NULL, szWindowClass, szTitle, WS_OVERLAPPEDWINDOW^WS_THICKFRAME,
												 CW_USEDEFAULT, CW_USEDEFAULT, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top,
												 NULL, NULL, (HINSTANCE)Context->instanceHandler, NULL );

	if( !Context->windowHandler )
	{
#ifdef _DEBUG
		LPVOID messageBuffer;
		DWORD error = GetLastError();
		FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
					   NULL, error, MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ),
					   (LPTSTR)&messageBuffer, 0, NULL );
		OutputDebugString( (LPCTSTR)messageBuffer );
		LocalFree( messageBuffer );
#endif
		return FALSE;
	}

	ShowAppWindow( nCmdShow );

	return TRUE;
}

void Engine::ShowAppWindow( int showFlags )
{
	ShowWindow( (HWND)Context->windowHandler, showFlags );
	UpdateWindow( (HWND)Context->windowHandler );
}

void Engine::HideAppWindow()
{
	ShowWindow( (HWND)Context->windowHandler, SW_HIDE );
}

//void Engine::HideWindow()
//{
//
//}

///@brief	W³aœciwa funkcja inicjujaca okno
///W trybie fullscreen szerokoœæ i wysokoœæ okna jest ignorowana, a dane s¹ pobierane z systemu.
///@param[in] width Szerokoœæ okna
///@param[in] height Wysokoœæ okna
///@param[in] fullscreen Pe³ny ekran lub renderowanie w oknie
///@param[in] nCmdShow Czwarty parametr funkcji WinMain
BOOL Engine::InitWindow( int width, int height, BOOL fullscreen, int nCmdShow )
{
	Context->windowHeight = height;
	Context->windowWidth = width;
	if( fullscreen )
		Context->fullScreen = true;		//zostal zainicjonwany w konstruktorze jako false

	if( Context->fullScreen )
	{
		Context->windowHeight = GetSystemMetrics( SM_CYSCREEN );
		Context->windowWidth = GetSystemMetrics( SM_CXSCREEN );
	}

	EngineRegisterClass();

	return InitInstance( nCmdShow );
}


///@brief Funkcja wstawia do kolejki komunikatów aplikacji wiadomoœc QUIT_MESSAGE.
void Engine::EndAplication()
{
	PostQuitMessage( 0 );
}


//----------------------------------------------------------------------------------------------//
//								main program loop												//
//----------------------------------------------------------------------------------------------//
/**
@page MainLoop Pêtla g³ówna silnika

@section Contents Spis Treœci
- @ref Code
- @ref Description


@subsection Code Kod
@code
int Engine::MainLoop()
{
	MSG msg;

#ifndef __UNUSED
	//w³¹czamy g³ówny w¹tek do renderingu
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
			join_render_thread = true;	//to jest jedyne miejsce, w którym ta zmienna mo¿e zostac ustawiona
										//dlatego mo¿emy to zrobiæ bez synchronizacji
			main_thread.join();
#endif
			break;
		}
	}

	return (int)msg.wParam;
}
@endcode
@subsection Description Opis
Pêtla g³ówna silnika zastêpuje Windowsowsk¹ pêtlê komunikatów, wywo³uj¹c jednoczeœnie pêtlê g³ówn¹ silnika (zobacz temat: Potok przetwarzania obiektów).
Musimy obs³ugiwaæ komunikaty windowsa, poniewa¿ w przeciwnym razie móg³by on uznaæ, ¿e nasz program siê zawiesi³, i spróbowaæ zabiæ nasz proces.
Dlatego pobieramy wiadomoœci z kolejki komunikatów i przekazujemy do dalszego przetwarzania.
Do pobierania komunikatów u¿ywamy nieblokuj¹cej funkcji PeekMessage (nie GetMessage).

Jako pêtla g³ówna silnika s³u¿y funkcja render_frame. Jest ona wywo³ywana tylko pod warunkiem, ¿e wczeœniejsza inicjalizacja
directXa w pe³ni powiod³a siê. Wa¿nym punktem jest inicjacja zmiennej time_previous. Niezainicjowanie tej zmiennej, mog³oby
spowodowaæ, ¿e w pierwszej klatce nast¹pi³aby nieprzewidywalna zmiana po³o¿eñ i orientacji wszystkich obiektów na scenie.

Wyjœcie z aplikacji nastêpuje kiedy w kolejce komunikatów windowsa znajdzie siê komunikat WM_QUIT. Z wnêtrza silnika mo¿na to osi¹gn¹æ przez wywo³anie funkcji Engine::EndAplication.

Definicja: "EngineWindowFunctions.cpp"
*/

/**@brief G³ówna pêtla programu.
@ref MainLoop*/
//int Engine::MainLoop()
//{
//	MSG msg;
//
//#ifndef __UNUSED
//	//w³¹czamy g³ówny w¹tek do renderingu
//	std::thread main_thread( main_thread_function, this );
//#endif
//
//	// Main message loop:
//	while( TRUE )
//	{
//		if( Context->engineReady )
//			RenderFrame();
//
//		while( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
//		//while (GetMessage(&msg, NULL, 0, 0))
//		{
//			TranslateMessage( &msg );
//			DispatchMessage( &msg );
//		}
//
//		if( msg.message == WM_QUIT )
//		{
//#ifndef __UNUSED
//			join_render_thread = true;	//to jest jedyne miejsce, w którym ta zmienna mo¿e zostac ustawiona
//										//dlatego mo¿emy to zrobiæ bez synchronizacji
//			main_thread.join();
//#endif
//			break;
//	}
//}
//
//	return (int)msg.wParam;
//}

#ifndef __UNUSED
/*Funkcje oddzielaj¹ce w¹tek obs³ugi komunikatów od pêtli g³ównej.
Myœla³em przez chwilê, ¿e takie oddzielenie jest konieczne, ale okaza³o siê,
¿e to nie rozwi¹zuje problemów, a tylko dodaje nowe.
Utrudnia to zakoñczenie programu, poniewa¿ wys³anie wiadomoœci WM_QUIT z innego
w¹tku wymaga wiêcej roboty.

Funkcje pozstawiam, poniewa¿ mog¹ siê w przysz³oœci przydaæ.
Wielow¹tkowoœæ jest nieuchronna, ale mo¿e niekoniecznie w tej postaci.*/

void Engine::render_thread()
{
	int threads_num = get_efficient_threads_count();


	while( TRUE )
	{
		//Jedynie funkcja Engine::MainLoop ma prawo modyfikowaæ tê zmienn¹.
		//Nie jest potrzebna synchronizacja. Najwy¿ej zakoñczenie w¹tku opóŸni siê o jedna ramkê.
		if( join_render_thread )
		{
			wait_for_threads();		//koñczymy inne w¹tki
			return;
		}

		if( directX_ready )
			RenderFrame();
	}
}

/*
Uruchamiamy w¹tek odpowiedzialny za pêtlê renderingu. Jest to g³ówny w¹tek programu,
który zosta³ oddzielony od w¹tku obs³uguj¹cego komunikaty windowsa, aby nie zatrzymywa³
siê na funkcjach je obs³uguj¹cych (nie wystarczy³o zmieniæ GetMessage na PeekMessage, jakieœ
wbudowane funkcje obs³ugi komunikatów powodowa³y, ¿e procesor wiêkszoœæ czasu sta³).

Ta funkcja istnieje tylko po to, ¿eby j¹ mo¿na by³o podaæ jako wskaŸnik przy tworzeniu
nowego w¹tku. Nie nale¿y tutaj dodawaæ ju¿ niczego.*/
void main_thread_function( Engine* engine )
{
	engine->render_thread();
}

//----------------------------------------------------------------------------------------------//
//								helper functions												//
//----------------------------------------------------------------------------------------------//

/*
#do zaimplementowania

Zwraca liczbê w¹tków jak¹ najlepiej stworzyæ, ¿eby wykorzystanie procesora by³o
najbardziej wydajne. Liczba ta zale¿y od liczby rdzeni procesora oraz liczby w¹tków,
które mog¹ byc jednoczeœnie liczone na jednym rdzeniu.*/
unsigned int Engine::get_efficient_threads_count()
{
	return 1;
}

/*
#do zaimplementowania

Funkcja ma za zadanie zakoñczyæ wykonanie wszystkich w¹tków, które zosta³y stworzone
przez w¹tek render_thread.
*/
void Engine::wait_for_threads()
{

}

#endif


}	// sw


//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//

//----------------------------------------------------------------------------------------------//
//								window procedure												//
//----------------------------------------------------------------------------------------------//

LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch( message )
	{
		case WM_PAINT:
			hdc = BeginPaint( hWnd, &ps );
			// TODO: Add any drawing code here...
			EndPaint( hWnd, &ps );
			break;
		case WM_DESTROY:
			PostQuitMessage( 0 );
			break;
		default:
			return DefWindowProc( hWnd, message, wParam, lParam );
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam )
{
	UNREFERENCED_PARAMETER( lParam );
	switch( message )
	{
		case WM_INITDIALOG:
			return (INT_PTR)TRUE;

		case WM_COMMAND:
			if( LOWORD( wParam ) == IDOK || LOWORD( wParam ) == IDCANCEL )
			{
				EndDialog( hDlg, LOWORD( wParam ) );
				return (INT_PTR)TRUE;
			}
			break;
	}
	return (INT_PTR)FALSE;
}



