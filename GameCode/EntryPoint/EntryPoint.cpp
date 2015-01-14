/*To jest szablon g³ównego pliku projektu SW_engine.
Zawiera funkcjê winmain, w której nastêpuje stworzenie obiektu g³ównego silnika,
inicjalizacja DirectX, DirectInput oraz podanie klasy GamePlay, któr¹ silnik
ma wczytaæ na pocz¹tku.

Nastêpnie jest wywo³ywana g³ówna pêtla silnika.
Pêtla komunikatów Windowsa jest obs³ugiwana wewn¹trz tej funkcji.

Ten plik nie powinien byæ modyfikowany. Jedyna zmiana jakiej mo¿na dokonaæ,
to podanie innego obiektu GamePlay.

Ca³y kod gry powinien znajdowaæ siê w innych plikach.*/


#include "stdafx.h"
#include "Engine.h"


/*Ustaw rozdzielczoœæ okna w jakim ma byæ wyœwietlana gra. Je¿eli ma byæ w trybie 
pe³noekranowym, wartoœæ tych parametrów mo¿e pozostaæ dowolna.*/
#define WINDOW_WIDTH		1280
#define WINDOW_HEIGHT		800


// WskaŸnik na g³ówny obiekt silnika
Engine* engine;


int APIENTRY _tWinMain( _In_ HINSTANCE hInstance,
						_In_opt_ HINSTANCE hPrevInstance,
						_In_ LPTSTR    lpCmdLine,
						_In_ int       nCmdShow )
{
	UNREFERENCED_PARAMETER( hPrevInstance );
	UNREFERENCED_PARAMETER( lpCmdLine );

	int result;

	engine = new Engine( hInstance );

	// Tworzymy g³ówne okno aplikacji. Dla trybu pe³noekranowego nale¿y podaæ w trzecim parametrze wartoœæ TRUE
	result = engine->init_window( WINDOW_WIDTH, WINDOW_HEIGHT, FALSE, nCmdShow );
	if ( !result )
	{
		delete engine;
		return FALSE;
	}

	// Inicjalizacja zmiennych DirectXa
	result = engine->init_directX( );
	if ( result != GRAPHIC_ENGINE_INIT_OK )
	{
		delete engine;
		return FALSE;
	}
	// Inicjalizacja directXinputa
	result = engine->init_directXinput( );
	if ( result != DIRECT_INPUT_OK )
	{
		delete engine;
		return FALSE;
	}

	engine->test( );

	result = engine->main_loop( );
	delete engine;
	return result;
}

