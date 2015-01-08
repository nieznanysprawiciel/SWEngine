// StarWars_engine.cpp : Defines the entry point for the application.
//
#include "stdafx.h"
#include "Engine\Engine.h"



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
	result = engine->init_window(WINDOW_WIDTH, WINDOW_HEIGHT, FALSE, nCmdShow);
	if (!result)
	{
		delete engine;
		return FALSE;
	}

	//initializing directX
	result = engine->init_directX();
	if (result != GRAPHIC_ENGINE_INIT_OK)
	{
		delete engine;
		return FALSE;
	}
	//initializing directXinput
	result = engine->init_directXinput();
	if (result != DIRECT_INPUT_OK)
	{
		delete engine;
		return FALSE;
	}

	engine->test();

	result = engine->main_loop();
	delete engine;
	return result;
}

