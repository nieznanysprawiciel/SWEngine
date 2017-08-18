/**@file EngineWindowFunctions.cpp
@author nieznanysprawiciel
@copyright Plik jest cz�ci� silnika graficznego SWEngine.

@brief Plik zawiera definicje metod klasy Engine dotycz�ce tworzenia okna, obs�ugi p�tli komunikat�w
WinAPI oraz g��wn� p�tl� programu @ref Engine::MainLoop.
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

/**@brief Rejestruje klas� okna aplikacji.*/
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
Okno nie jest pokazywane na ekranie. Do tego trzeba u�y� funkcji Engine::ShowAppWindow.

@return Zwraca TRUE, je�eli inicjowanie okna powiod�o si�.*/
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

///@brief	W�a�ciwa funkcja inicjujaca okno
///W trybie fullscreen szeroko�� i wysoko�� okna jest ignorowana, a dane s� pobierane z systemu.
///@param[in] width Szeroko�� okna
///@param[in] height Wysoko�� okna
///@param[in] fullscreen Pe�ny ekran lub renderowanie w oknie
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


///@brief Funkcja wstawia do kolejki komunikat�w aplikacji wiadomo�c QUIT_MESSAGE.
void Engine::EndAplication()
{
	PostQuitMessage( 0 );
}


//----------------------------------------------------------------------------------------------//
//								main program loop												//
//----------------------------------------------------------------------------------------------//
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

Definicja: "EngineWindowFunctions.cpp"
*/

/**@brief G��wna p�tla programu.
@ref MainLoop*/
//int Engine::MainLoop()
//{
//	MSG msg;
//
//#ifndef __UNUSED
//	//w��czamy g��wny w�tek do renderingu
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
//			join_render_thread = true;	//to jest jedyne miejsce, w kt�rym ta zmienna mo�e zostac ustawiona
//										//dlatego mo�emy to zrobi� bez synchronizacji
//			main_thread.join();
//#endif
//			break;
//	}
//}
//
//	return (int)msg.wParam;
//}

#ifndef __UNUSED
/*Funkcje oddzielaj�ce w�tek obs�ugi komunikat�w od p�tli g��wnej.
My�la�em przez chwil�, �e takie oddzielenie jest konieczne, ale okaza�o si�,
�e to nie rozwi�zuje problem�w, a tylko dodaje nowe.
Utrudnia to zako�czenie programu, poniewa� wys�anie wiadomo�ci WM_QUIT z innego
w�tku wymaga wi�cej roboty.

Funkcje pozstawiam, poniewa� mog� si� w przysz�o�ci przyda�.
Wielow�tkowo�� jest nieuchronna, ale mo�e niekoniecznie w tej postaci.*/

void Engine::render_thread()
{
	int threads_num = get_efficient_threads_count();


	while( TRUE )
	{
		//Jedynie funkcja Engine::MainLoop ma prawo modyfikowa� t� zmienn�.
		//Nie jest potrzebna synchronizacja. Najwy�ej zako�czenie w�tku op�ni si� o jedna ramk�.
		if( join_render_thread )
		{
			wait_for_threads();		//ko�czymy inne w�tki
			return;
		}

		if( directX_ready )
			RenderFrame();
	}
}

/*
Uruchamiamy w�tek odpowiedzialny za p�tl� renderingu. Jest to g��wny w�tek programu,
kt�ry zosta� oddzielony od w�tku obs�uguj�cego komunikaty windowsa, aby nie zatrzymywa�
si� na funkcjach je obs�uguj�cych (nie wystarczy�o zmieni� GetMessage na PeekMessage, jakie�
wbudowane funkcje obs�ugi komunikat�w powodowa�y, �e procesor wi�kszo�� czasu sta�).

Ta funkcja istnieje tylko po to, �eby j� mo�na by�o poda� jako wska�nik przy tworzeniu
nowego w�tku. Nie nale�y tutaj dodawa� ju� niczego.*/
void main_thread_function( Engine* engine )
{
	engine->render_thread();
}

//----------------------------------------------------------------------------------------------//
//								helper functions												//
//----------------------------------------------------------------------------------------------//

/*
#do zaimplementowania

Zwraca liczb� w�tk�w jak� najlepiej stworzy�, �eby wykorzystanie procesora by�o
najbardziej wydajne. Liczba ta zale�y od liczby rdzeni procesora oraz liczby w�tk�w,
kt�re mog� byc jednocze�nie liczone na jednym rdzeniu.*/
unsigned int Engine::get_efficient_threads_count()
{
	return 1;
}

/*
#do zaimplementowania

Funkcja ma za zadanie zako�czy� wykonanie wszystkich w�tk�w, kt�re zosta�y stworzone
przez w�tek render_thread.
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



