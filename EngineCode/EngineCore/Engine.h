#pragma once

/**@file Engine.h
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.

@brief Plik zawiera deklaracjê g³ównego obiektu silnika.*/

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




/**@brief Enumeracja zawieraj¹ca identyfikatory dla poszczegolnych modu³ów silnika.*/
typedef enum
{
	E_CONTROLLERS = 0,			///<Klasa ControllersEngine
	E_PHYSIC = 1,				///<Klasa PhysicEngine
	E_FABLE = 2,				///<Klasa FableEngine
	E_MOVEMENT_DYNAMIC = 3,		///<Obiekty dynamiczne w klasie MovementEngine
	E_MOVEMENT_COMPLEX = 4,		///<Obiekty z³o¿one w klasie MovementEngine
	E_DISPLAY = 5,				///<Klasa DisplayEngine
	E_COLLISION = 6,			///<Klasa CollisionEngine
	E_UI = 7,					///<Klasa UI_engine
	E_MODELS_MANAGER = 8		///<Klasa ModelsManager
} Modules;



//global variables
extern TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
extern TCHAR szWindowClass[MAX_LOADSTRING];				// the main window class name


/**@brief Klasa Engine jest nadrzêdnym obiektem zarz¹dzaj¹cym wszystkimi modu³ami silnika.
W aplikacji powinien istnieæ tylko jeden obiekt podanego typu. O tym, jak siê inicjuje dzia³anie silnika,
mo¿na przeczytaæ w temacie Inicjowanie silnika.

W tej klasie powinny byæ umieszczane wszystkie funkcje, które nie powinny byæ dostêpne
dla u¿ytkownika silnika. Interfejsem dla u¿ytkownika jest klasa engine_interface i tam nale¿y umieszczaæ
pozosta³e funkcje. Co do zmiennych klasy, wszystki powinny siê znaleŸæ w klasie EnigneInterface, aby wszystkie
funkcje mog³y mieæ do nich dostêp.
*/
class Engine : public EngineInterface
{
private:		//zmienne, które nie maj¹ prawa zostaæ u¿yte przez engine_interface


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

/**Celem wprowadzenia wektora index_predictor jest optymalizacja dostêpu do danych 
 *obiektów w modu³ach silnika. Ka¿dy modu³ zawiera tablicê obiektów, które ma obs³ugiwaæ.
 *Obiekty mog¹ byæ do tych tablic dodawane i usuwane.
 *Z tego powodu nie jesteœmy w stanie ca³y czas przechowywaæ aktualnego indeksu obiektów w kazdym module,
 *ale charakterystyka operacji, które wykonujemy, pozwala na przewidzenie, na jakiej pozycji mniej wiêcej znajduje siê
 *dany obiekt.
 *Zauwa¿my, ¿e je¿eli umieœcimy jakiœ obiekt na pozycji n-tej, to ten indeks nigdy nie bêdzie wiêkszy, bo nigdy nie wstawiamy
 *elementów w œrodek tablicy, tylko zawsze na koniec. Kasuj¹c elementy wczesniejsze, przesuwamy obiekt w kierunku numerów mniejszych od n.
 *Z tego wynika, ¿e najbardziej efektywne bêdzie przeszukiwanie od pozycji n-tej w dó³.
 *
 *Regu³a ta nie dzia³a w przypadku klasy DisplayEngine. Tablica jest tam u³o¿ona w celu optymalizacji wyœwietlania, co oznacza,
 *¿e niektóre obiekty trzeba bêdzie wstawiaæ w œrodek tablicy.
 *W tej sytuacji wyjœciem jest przeszukiwanie dwukierunkowe od indeksu z numerem n.
 *
 *Ponadto, aby trzymaæ tablicê index_predictor jak najbardziej aktualn¹, ka¿da funkcja, która odwo³uje siê do danego elementu
 *powinna aktualizowac jego indeks.
*/
struct IndexPrediction
{
	Object*				object_ptr;
	unsigned int		index[NUM_OF_MODULES];
};
