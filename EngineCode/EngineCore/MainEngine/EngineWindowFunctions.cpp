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


}	// sw
