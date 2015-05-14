#include "stdafx.h"
#include "Engine.h"

/**@file SW_engine.cpp
@brief Plik zawiera funkcjê WinMain i instrukcje tworz¹ce obiekt silnika, inicjuj¹ce go
i wywo³uj¹ce pêtlê g³ówn¹ aplikacji.*/

#define WINDOW_WIDTH		1280
#define WINDOW_HEIGHT		800


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

	int result;

	engine = new Engine(hInstance);

	//creating aplication window
	result = engine->init_engine(WINDOW_WIDTH, WINDOW_HEIGHT, FALSE, nCmdShow);
	if (!result)
	{
		delete engine;
		return FALSE;
	}

#ifdef __TEST
	engine->test();
#endif

	result = engine->main_loop();
	delete engine;

#ifdef SHOW_MEMORY_LEAKS
	_CrtDumpMemoryLeaks( );
#endif
	return result;
}

