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



#include <thread>



#include "swCommonLib/Common/MemoryLeaks.h"




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



//----------------------------------------------------------------------------------------------//
//								window functions												//
//----------------------------------------------------------------------------------------------//


/**@brief Requests native gui to end application.*/
void		Engine::EndAplication()
{
	m_nativeGUI->EndApplication();
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


}	// sw
