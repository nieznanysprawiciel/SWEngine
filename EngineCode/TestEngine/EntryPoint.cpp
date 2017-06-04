/*To jest szablon g³ównego pliku projektu SW_engine.
Zawiera funkcjê winmain, w której nastêpuje stworzenie obiektu g³ównego silnika,
inicjalizacja DirectX, DirectInput oraz podanie klasy IGamePlay, któr¹ silnik
ma wczytaæ na pocz¹tku.

Nastêpnie jest wywo³ywana g³ówna pêtla silnika.
Pêtla komunikatów Windowsa jest obs³ugiwana wewn¹trz tej funkcji.

Ten plik nie powinien byæ modyfikowany. Jedyna zmiana jakiej mo¿na dokonaæ,
to podanie innego obiektu IGamePlay.

Ca³y kod gry powinien znajdowaæ siê w innych plikach.*/


#include "stdafx.h"
#include "EngineCore/MainEngine/Engine.h"
#include "GamePlayCode/EntryPointGamePlay.h"
//#include "EngineEditor/LightmapsTool/LightmapLogic.h"

#include "swCommonLib/Common/MemoryLeaks.h"

/*Ustaw rozdzielczoœæ okna w jakim ma byæ wyœwietlana gra. Je¿eli ma byæ w trybie 
pe³noekranowym, wartoœæ tych parametrów mo¿e pozostaæ dowolna.*/
#define WINDOW_WIDTH		1280
#define WINDOW_HEIGHT		800


// WskaŸnik na g³ówny obiekt silnika
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
												//UWAGA2! Zmienna musi byæ alokowana na stercie, operatorem new z tego samego powodu co wy¿ej

	// Tworzymy g³ówne okno aplikacji, inicjalizujemy DirectX, Directinput i DirectSound.
	//Dla trybu pe³noekranowego nale¿y podaæ w trzecim parametrze wartoœæ TRUE. Wysokoœæ i szerokoœæ
	//okna s¹ w takim przypadku ignorowane.
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

