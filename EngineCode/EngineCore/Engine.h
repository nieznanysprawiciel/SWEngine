#pragma once

/**@file Engine.h
@author nieznanysprawiciel
@copyright Plik jest cz�ci� silnika graficznego SWEngine.

@brief Plik zawiera deklaracj� g��wnego obiektu silnika.*/

#include "SW_engine.h"
#include "Interfaces/basic_interfaces.h"
#include "Interfaces/EngineInterface.h"
#include "Events/Event.h"
#include "CollisionEngine/CollisionEngine.h"
#include "ControllersEngine/ControllersEngine.h"
#include "FableEngine/FableEngine.h"
#include "DisplayEngine/DisplayEngine.h"
#include "ModelsManager/ModelsManager.h"
#include "PhysicEngine/PhysicEngine.h"
#include "PhysicEngine/MovementEngine.h"
#include "SoundEngine/SoundEngine.h"
#include "UI_Engine/UI_Engine.h"



//	window defines
#define		MAX_LOADSTRING			100

//	engines defines
#define		NUM_OF_MODULES			9




/**@brief Enumeracja zawieraj�ca identyfikatory dla poszczegolnych modu��w silnika.*/
typedef enum
{
	E_CONTROLLERS = 0,			///<Klasa ControllersEngine
	E_PHYSIC = 1,				///<Klasa PhysicEngine
	E_FABLE = 2,				///<Klasa FableEngine
	E_MOVEMENT_DYNAMIC = 3,		///<Obiekty dynamiczne w klasie MovementEngine
	E_MOVEMENT_COMPLEX = 4,		///<Obiekty z�o�one w klasie MovementEngine
	E_DISPLAY = 5,				///<Klasa DisplayEngine
	E_COLLISION = 6,			///<Klasa CollisionEngine
	E_UI = 7,					///<Klasa UI_engine
	E_MODELS_MANAGER = 8		///<Klasa ModelsManager
} Modules;



//global variables
extern TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
extern TCHAR szWindowClass[MAX_LOADSTRING];				// the main window class name


/**@brief Klasa Engine jest nadrz�dnym obiektem zarz�dzaj�cym wszystkimi modu�ami silnika.
W aplikacji powinien istnie� tylko jeden obiekt podanego typu. O tym, jak si� inicjuje dzia�anie silnika,
mo�na przeczyta� w temacie Inicjowanie silnika.

W tej klasie powinny by� umieszczane wszystkie funkcje, kt�re nie powinny by� dost�pne
dla u�ytkownika silnika. Interfejsem dla u�ytkownika jest klasa engine_interface i tam nale�y umieszcza�
pozosta�e funkcje. Co do zmiennych klasy, wszystki powinny si� znale�� w klasie EnigneInterface, aby wszystkie
funkcje mog�y mie� do nich dost�p.
*/
class Engine : public EngineInterface
{
private:		//zmienne, kt�re nie maj� prawa zosta� u�yte przez engine_interface


private:
	//window functions
	ATOM MyRegisterClass();
	BOOL InitInstance(int);

	//DirectX functions
	void clean_DirectX();

	//rendering
	void render_frame();


#ifndef __UNUSED
	//render thread joining
	bool join_render_thread;
#endif

public:
	Engine(HINSTANCE instance);
	~Engine();

	//DirectX
	int init_directX();
	int init_directXinput();

	//window fuctions
	int init_engine( int width, int height, bool full_screen, int nCmdShow );
	BOOL init_window(int width, int height, BOOL full_screen, int nCmdShow);
	void end_aplication();
	int main_loop();

#ifndef __UNUSED
	//threads
	void render_thread();
	unsigned int get_efficient_threads_count();
	void wait_for_threads();
#endif


	//Engine functions
#ifndef __UNUSED
	void time_controller(float& time_interval);
#endif

	//event functions
	void							send_event				(Event* new_event);

	void							set_entry_point			( GamePlay* game_play );
	void							set_entry_point			( const std::wstring dll_name );

#ifdef __TEST
	void test();
	std::vector<Object*>	object_list;
#endif


	inline std::queue<Event*>*		get_events_queue()									{ return events_queue; }
	inline void						set_events_queue( std::queue<Event*>* queue )		{ events_queue = queue; }
	inline HINSTANCE				get_instance_handler()								{ return instance_handler; }
	inline HWND						get_window_handler()								{ return window_handler; }

	inline int						get_window_widht()									{ return window_width; }
	inline int						get_window_height()									{ return window_height; }
};

/**Celem wprowadzenia wektora index_predictor jest optymalizacja dost�pu do danych 
 *obiekt�w w modu�ach silnika. Ka�dy modu� zawiera tablic� obiekt�w, kt�re ma obs�ugiwa�.
 *Obiekty mog� by� do tych tablic dodawane i usuwane.
 *Z tego powodu nie jeste�my w stanie ca�y czas przechowywa� aktualnego indeksu obiekt�w w kazdym module,
 *ale charakterystyka operacji, kt�re wykonujemy, pozwala na przewidzenie, na jakiej pozycji mniej wi�cej znajduje si�
 *dany obiekt.
 *Zauwa�my, �e je�eli umie�cimy jaki� obiekt na pozycji n-tej, to ten indeks nigdy nie b�dzie wi�kszy, bo nigdy nie wstawiamy
 *element�w w �rodek tablicy, tylko zawsze na koniec. Kasuj�c elementy wczesniejsze, przesuwamy obiekt w kierunku numer�w mniejszych od n.
 *Z tego wynika, �e najbardziej efektywne b�dzie przeszukiwanie od pozycji n-tej w d�.
 *
 *Regu�a ta nie dzia�a w przypadku klasy DisplayEngine. Tablica jest tam u�o�ona w celu optymalizacji wy�wietlania, co oznacza,
 *�e niekt�re obiekty trzeba b�dzie wstawia� w �rodek tablicy.
 *W tej sytuacji wyj�ciem jest przeszukiwanie dwukierunkowe od indeksu z numerem n.
 *
 *Ponadto, aby trzyma� tablic� index_predictor jak najbardziej aktualn�, ka�da funkcja, kt�ra odwo�uje si� do danego elementu
 *powinna aktualizowac jego indeks.
*/
struct IndexPrediction
{
	Object*				object_ptr;
	unsigned int		index[NUM_OF_MODULES];
};
