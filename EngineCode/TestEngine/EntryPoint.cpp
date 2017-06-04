/*To jest szablon g��wnego pliku projektu SW_engine.
Zawiera funkcj� winmain, w kt�rej nast�puje stworzenie obiektu g��wnego silnika,
inicjalizacja DirectX, DirectInput oraz podanie klasy IGamePlay, kt�r� silnik
ma wczyta� na pocz�tku.

Nast�pnie jest wywo�ywana g��wna p�tla silnika.
P�tla komunikat�w Windowsa jest obs�ugiwana wewn�trz tej funkcji.

Ten plik nie powinien by� modyfikowany. Jedyna zmiana jakiej mo�na dokona�,
to podanie innego obiektu IGamePlay.

Ca�y kod gry powinien znajdowa� si� w innych plikach.*/


#include "stdafx.h"
#include "EngineCore/MainEngine/Engine.h"
#include "GamePlayCode/EntryPointGamePlay.h"
//#include "EngineEditor/LightmapsTool/LightmapLogic.h"

#include "swCommonLib/Common/MemoryLeaks.h"

/*Ustaw rozdzielczo�� okna w jakim ma by� wy�wietlana gra. Je�eli ma by� w trybie 
pe�noekranowym, warto�� tych parametr�w mo�e pozosta� dowolna.*/
#define WINDOW_WIDTH		1280
#define WINDOW_HEIGHT		800


// Wska�nik na g��wny obiekt silnika
sw::Engine* engine;


int APIENTRY _tWinMain( _In_ HINSTANCE hInstance,
						_In_opt_ HINSTANCE hPrevInstance,
						_In_ LPTSTR    lpCmdLine,
						_In_ int       nCmdShow )
{
	UNREFERENCED_PARAMETER( hPrevInstance );
	UNREFERENCED_PARAMETER( lpCmdLine );

#ifdef SHOW_MEMORY_LEAKS
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	int result;

	engine = new sw::Engine();
	EntryPointGamePlay* entryPoint = new EntryPointGamePlay();	//UWAGA!! nie kasujemy zmiennej, robi to silnik.
												//UWAGA2! Zmienna musi by� alokowana na stercie, operatorem new z tego samego powodu co wy�ej

	// Tworzymy g��wne okno aplikacji, inicjalizujemy DirectX, Directinput i DirectSound.
	//Dla trybu pe�noekranowego nale�y poda� w trzecim parametrze warto�� TRUE. Wysoko�� i szeroko��
	//okna s� w takim przypadku ignorowane.
	result = engine->InitEngine( nCmdShow );
	if ( !result )
	{
		delete engine;
		return FALSE;
	}

	engine->SetEntryPoint( entryPoint );
	engine->test();		//to potem zniknie

	result = engine->MainLoop( );
	delete engine;
	return result;
}

