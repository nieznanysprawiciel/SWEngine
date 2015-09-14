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
#include "class_code\EntryPoint_class.h"


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
	EntryPoint* entry_point = new EntryPoint();	//UWAGA!! nie kasujemy zmiennej, robi to silnik.
												//UWAGA2! Zmienna musi byæ alokowana na stercie, operatorem new z tego samego powodu co wy¿ej

	// Tworzymy g³ówne okno aplikacji, inicjalizujemy DirectX, Directinput i DirectSound.
	//Dla trybu pe³noekranowego nale¿y podaæ w trzecim parametrze wartoœæ TRUE. Wysokoœæ i szerokoœæ
	//okna s¹ w takim przypadku ignorowane.
	result = engine->InitEngine( WINDOW_WIDTH, WINDOW_HEIGHT, false, nCmdShow );
	if ( !result )
	{
		delete engine;
		return FALSE;
	}

	engine->set_entry_point( entry_point );
	engine->test();		//to potem zniknie

	result = engine->main_loop( );
	delete engine;
	return result;
}

