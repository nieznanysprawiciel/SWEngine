#pragma once

/**@file EngineInterface.h
@brief Plik zawiera deklaracj� klasy engine_interface.*/

#include <queue>
#include "..\Engine\DX11_interfaces_container.h"




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


/**@brief Klasa jest interfejsem dla u�ytkownika u�ytkuj�cego silnik.

Taki interfejs b�dzie widzia� u�ytkownik silnika z wn�trza swoich klas odpowiedzialnych za fabu��.
U�ytkownik musi mie� ograniczone mo�liwo�ci, �eby przypadkiem nie namiesza�.
Szczeg�lnie nie powinien mie� mo�liwo�ci wysy�ania event�w, bo mog�oby to doprowadzi� do zap�tlenia.
Wszystkie funkcje jakie powinien mie� dost�pne nale�y deklarowa� w tej klasie w sekcji publicznej.
Wszystkie pozosta�e funkcje powinny si� znale�� w deklaracji klasy Engine.
Wszystkie zmienne silnika powinny by� deklarowane na tym poziomie, aby r�wnie� metody tej klasy dost�pne
dla u�ytkownika mia�y do nich dost�p.
*/
class engine_interface : public DX11_constant_buffers_container
{
	// Dziedziczymy z DX11_constant_buffers_container a nie z DX11_interfaces_container, poniewa�
	// musimy wywo�a� funkcj� release_DirectX z klasy pochodnej. Inaczej nie zwolnimy tamtych obiekt�w wog�le.

protected:
	//directX and windows variables
	bool						directX_ready;			///<Je�eli zmienna jest niepoprawna, nie renderujemy

	bool						full_screen;			///<Pe�ny ekran lub renderowanie w oknie
	HWND						window_handler;			///<Uchwyt okna aplikacji
	HINSTANCE					instance_handler;		///<Uchwyt instancji procesu
	int							window_width;			///<Szeroko�� okna/ekranu
	int							window_height;			///<Wysoko�� okna/ekranu

	//Engines
	ControllersEngine*			controllers_engine;		///<kontroluje AI (klawiatur� trzeba przenie��)
	MovementEngine*				movement_engine;		///<przelicza pozycj� obiekt�w w nast�pnej klatce
	DisplayEngine*				display_engine;			///<wy�wietla obiekty na scenie
	CollisionEngine*			collision_engine;		///<liczy kolizje
	PhysicEngine*				physic_engine;			///<liczy oddzia�ywania fizyczne
	ModelsManager*				models_manager;			///<zarz�dza modelami, teksturami i materia�ami
	FableEngine*				fable_engine;			///<zarz�dza fabu�� gry, interakcjami obiekt�w itd. Odpowiada za tre��
	SoundEngine*				sound_engine;			///<zarz�dza muzyk� i d�wi�kami
	UI_Engine*					ui_engine;				///<interfejs u�ytkownika (tak�e graficzny)

	//Objects
	//std::vector<IndexPrediction>		index_predictor;

	//time control
	__int64						timer_frequency;///<cz�stotliwo�� timera
	__int64						time_previous;	///<b�dziemy mierzy� r�nic� miedzy czasami w kolejnych
	__int64						elapsed_time;	///<do licznika FPS�w, czas od ostatniej aktualizacji frames_per_sec
	float						lag;			///<op�nienie wzgl�dem normalego tempa updatowania sceny
	unsigned int				frames;			///<zlicza ramki od ostatniego wyzerowania
	float						frames_per_sec;	///<najwa�niejszy licznik w grze

	bool						pause;			///<Pauza

	//queue
	std::queue<Event*>*			events_queue;	///<Wska�nik na kolejke komunikat�w w celu szybszego dost�pu

protected:
	~engine_interface(){};

	//FableEngine - ta klasa jest dost�pna bezpo�rednio z GamePlay

public:


};