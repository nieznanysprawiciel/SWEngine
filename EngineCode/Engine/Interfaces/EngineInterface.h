#pragma once

/**@file EngineInterface.h
@brief Plik zawiera deklaracjê klasy engine_interface.*/

#include <queue>
#include "..\Engine\DX11_interfaces_container.h"
#include "MainEngine\TimeManager.h"




class ControllersEngine;
struct IndexPrediction;
class Event;

class ControllersEngine;
class ModelsManager;
class PhysicEngine;
class FableEngine;
class MovementEngine;
class DisplayEngine;
class CollisionEngine;
class SoundEngine;
class UI_Engine;
class Object;


/**@brief Klasa jest interfejsem dla u¿ytkownika u¿ytkuj¹cego silnik.

Taki interfejs bêdzie widzia³ u¿ytkownik silnika z wnêtrza swoich klas odpowiedzialnych za fabu³ê.
U¿ytkownik musi mieæ ograniczone mo¿liwoœci, ¿eby przypadkiem nie namiesza³.
Szczególnie nie powinien mieæ mo¿liwoœci wysy³ania eventów, bo mog³oby to doprowadziæ do zapêtlenia.
Wszystkie funkcje jakie powinien mieæ dostêpne nale¿y deklarowaæ w tej klasie w sekcji publicznej.
Wszystkie pozosta³e funkcje powinny siê znaleŸæ w deklaracji klasy Engine.
Wszystkie zmienne silnika powinny byæ deklarowane na tym poziomie, aby równie¿ metody tej klasy dostêpne
dla u¿ytkownika mia³y do nich dostêp.
*/
class engine_interface : public DX11_constant_buffers_container
{
	// Dziedziczymy z DX11_constant_buffers_container a nie z DX11_interfaces_container, poniewa¿
	// musimy wywo³aæ funkcjê release_DirectX z klasy pochodnej. Inaczej nie zwolnimy tamtych obiektów wogóle.

protected:
	//directX and windows variables
	bool						directX_ready;			///<Je¿eli zmienna jest niepoprawna, nie renderujemy

	bool						full_screen;			///<Pe³ny ekran lub renderowanie w oknie
	HWND						window_handler;			///<Uchwyt okna aplikacji
	HINSTANCE					instance_handler;		///<Uchwyt instancji procesu
	int							window_width;			///<Szerokoœæ okna/ekranu
	int							window_height;			///<Wysokoœæ okna/ekranu

	//Engines
	ControllersEngine*			controllers_engine;		///<Kontroluje AI (klawiaturê trzeba przenieœæ)
	MovementEngine*				movement_engine;		///<Przelicza pozycjê obiektów w nastêpnej klatce
	DisplayEngine*				display_engine;			///<Wyœwietla obiekty na scenie
	CollisionEngine*			collision_engine;		///<Liczy kolizje
	PhysicEngine*				physic_engine;			///<Liczy oddzia³ywania fizyczne
	ModelsManager*				models_manager;			///<Zarz¹dza modelami, teksturami i materia³ami
	FableEngine*				fable_engine;			///<Zarz¹dza fabu³¹ gry, interakcjami obiektów itd. Odpowiada za treœæ
	SoundEngine*				sound_engine;			///<Zarz¹dza muzyk¹ i dŸwiêkami
	UI_Engine*					ui_engine;				///<Interfejs u¿ytkownika (tak¿e graficzny)

	//Objects
	//std::vector<IndexPrediction>		index_predictor;

	TimeManager					time_manager;	///<Obiekt do zarz¹dzania czasem.

	bool						pause;			///<Pauza

	//queue
	std::queue<Event*>*			events_queue;	///<WskaŸnik na kolejke komunikatów w celu szybszego dostêpu

protected:
	~engine_interface(){};

	//FableEngine - ta klasa jest dostêpna bezpoœrednio z GamePlay

public:


};