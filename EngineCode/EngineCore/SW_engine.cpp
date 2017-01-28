#include "EngineCore/stdafx.h"

#ifndef _APPLICATION_

#include "EngineCore/MainEngine/Engine.h"

/**@file SW_engine.cpp
@brief Plik zawiera funkcj� WinMain i instrukcje tworz�ce obiekt silnika, inicjuj�ce go
i wywo�uj�ce p�tl� g��wn� aplikacji.*/

const unsigned int WINDOW_WIDTH = 1280;		///<Szeroko�� okna aplikacji.
const unsigned int WINDOW_HEIGHT = 800;		///<Wysoko�� okna aplikacji.


// Global Variables:

// engine variables
Engine* engine;


int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

#ifdef SHOW_MEMORY_LEAKS
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif


	int result;

	engine = new Engine();

	//creating aplication window
	result = engine->InitEngine( nCmdShow );
	if (!result)
	{
		delete engine;
		return FALSE;
	}

#ifdef __TEST
	engine->test();
#endif

	result = engine->MainLoop();
	delete engine;


	return result;
}

#endif _APPLICATION_
