/**@file Engine_window_functions.cpp
@brief Plik zawiera definicje metod klasy Engine dotycz¹ce tworzenia okna, obs³ugi pêtli komunikatów
WinAPI oraz g³ówn¹ pêtlê programu @ref Engine::main_loop.
*/

#include "stdafx.h"
#include "Engine.h"
#include <thread>



#include "memory_leaks.h"



//global variables
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

void main_thread_function(Engine* engine);


//----------------------------------------------------------------------------------------------//
//								window functions												//
//----------------------------------------------------------------------------------------------//

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM Engine::MyRegisterClass()
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = instance_handler;
	wcex.hIcon = LoadIcon(instance_handler, MAKEINTRESOURCE(IDI_SW_ENGINE));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(int)
//
//   PURPOSE: Saves instance handle and creates main window
//

BOOL Engine::InitInstance(int nCmdShow)
{
	if ( full_screen )
		window_handler = CreateWindowEx(NULL, szWindowClass, szTitle, WS_EX_TOPMOST | WS_POPUP,
										0, 0, window_width, window_height,
										NULL, NULL, instance_handler, NULL);
	else
		window_handler = CreateWindowEx( NULL, szWindowClass, szTitle, WS_OVERLAPPEDWINDOW^WS_THICKFRAME,
										CW_USEDEFAULT, 0, window_width, window_height,
										NULL, NULL, instance_handler, NULL);

	if (!window_handler)
		return FALSE;

	ShowWindow(window_handler, nCmdShow);
	UpdateWindow(window_handler);

	return TRUE;
}

///@brief	W³aœciwa funkcja inicjujaca okno
///W trybie fullscreen szerokoœæ i wysokoœæ okna jest ignorowana, a dane s¹ pobierane z systemu.
///@param[in] width Szerokoœæ okna
///@param[in] height Wysokoœæ okna
///@param[in] fullscreen Pe³ny ekran lub renderowanie w oknie
///@param[in] nCmdShow Czwarty parametr funkcji WinMain
BOOL Engine::init_window(int width, int height, BOOL fullscreen, int nCmdShow)
{
	window_height = height;
	window_width = width;
	if (fullscreen)
		full_screen = true;		//zostal zainicjonwany w konstruktorze jako false

	if (full_screen)
	{
		window_height = GetSystemMetrics(SM_CYSCREEN);
		window_width = GetSystemMetrics(SM_CXSCREEN);
	}
	
	MyRegisterClass();

	return InitInstance(nCmdShow);
}

///@brief Funkcja wstawia do kolejki komunikatów aplikacji wiadomoœc QUIT_MESSAGE
void Engine::end_aplication()
{
	PostQuitMessage( 0 );
}


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

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

//----------------------------------------------------------------------------------------------//
//								main program loop												//
//----------------------------------------------------------------------------------------------//

/**@brief G³ówna pêtla programu.
@ref MainLoop*/
int Engine::main_loop()
{
	MSG msg;

	time_manager.initTimer();		// Inicjujemy obiekt do mierzenia czasu.

#ifndef __UNUSED
	//w³¹czamy g³ówny w¹tek do renderingu
	std::thread main_thread(main_thread_function, this);
#endif

	// Main message loop:
	while (TRUE)
	{
		if ( directX_ready )
			render_frame();

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


	while (TRUE)
	{
		//Jedynie funkcja Engine::main_loop ma prawo modyfikowaæ tê zmienn¹.
		//Nie jest potrzebna synchronizacja. Najwy¿ej zakoñczenie w¹tku opóŸni siê o jedna ramkê.
		if (join_render_thread)
		{
			wait_for_threads();		//koñczymy inne w¹tki
			return;
		}

		if (directX_ready)
			render_frame();
	}
}

/*
Uruchamiamy w¹tek odpowiedzialny za pêtlê renderingu. Jest to g³ówny w¹tek programu,
który zosta³ oddzielony od w¹tku obs³uguj¹cego komunikaty windowsa, aby nie zatrzymywa³
siê na funkcjach je obs³uguj¹cych (nie wystarczy³o zmieniæ GetMessage na PeekMessage, jakieœ
wbudowane funkcje obs³ugi komunikatów powodowa³y, ¿e procesor wiêkszoœæ czasu sta³).

Ta funkcja istnieje tylko po to, ¿eby j¹ mo¿na by³o podaæ jako wskaŸnik przy tworzeniu
nowego w¹tku. Nie nale¿y tutaj dodawaæ ju¿ niczego.*/
void main_thread_function(Engine* engine)
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

