#pragma once

#include <queue>
#include "..\Engine\DX11_interfaces_container.h"

/*Taki interfejs bêdzie widzia³ u¿ytkownik silnika z wnêtrza swoich klas odpowiedzialnych za fabu³ê.
 *U¿ytkownik musi mieæ ograniczone mo¿liwoœci, ¿eby przypadkiem nie namiesza³. Szczególnie nie powinien
 *mieæ mo¿liwoœci wysy³ania eventów, bo mog³oby to doprowadziæ do zapêtlenia.*/


class ControllersEngine;
struct IndexPrediction;
class Event;


class engine_interface : public DX11_constant_buffers_container
{
	// Dziedziczymy z DX11_constant_buffers_container a nie z DX11_interfaces_container, poniewa¿
	// musimy wywo³aæ funkcjê release_DirectX z klasy pochodnej. Inaczej nie zwolnimy tamtych obiektów wogóle.

	friend class ControllersEngine;
	friend class ModelsManager;
	friend class PhysicEngine;
	friend class FableEngine;
	friend class MovementEngine;
	friend class DisplayEngine;
	friend class CollisionEngine;
	friend class SoundEngine;
	friend class UI_Engine;
	friend class Object;
protected:
	//directX and windows variables
	bool						directX_ready;			// je¿eli zmienna jest niepoprawna, nie renderujemy

	bool						full_screen;
	HWND						window_handler;
	HINSTANCE					instance_handler;
	int							window_width;
	int							window_height;

	//Engines
	ControllersEngine*			controllers_engine;		//kontroluje AI (klawiaturê trzeba przenieœæ)
	MovementEngine*				movement_engine;		//przelicza pozycjê obiektów w nastêpnej klatce
	DisplayEngine*				display_engine;			//wyœwietla obiekty na scenie
	CollisionEngine*			collision_engine;		//liczy kolizje
	PhysicEngine*				physic_engine;			//liczy oddzia³ywania fizyczne
	ModelsManager*				models_manager;			//zarz¹dza modelami, teksturami i materia³ami
	FableEngine*				fable_engine;			//zarz¹dza fabu³¹ gry, interakcjami obiektów itd. Odpowiada za treœæ
	SoundEngine*				sound_engine;			//zarz¹dza muzyk¹ i dŸwiêkami
	UI_Engine*					ui_engine;				//interfejs u¿ytkownika (tak¿e graficzny)

	//Objects
	//std::vector<IndexPrediction>		index_predictor;

	//time control
	__int64						timer_frequency;//czêstotliwoœæ timera
	__int64						time_previous;	//bêdziemy mierzyæ ró¿nicê miedzy czasami w kolejnych
	__int64						elapsed_time;	//do licznika FPSów, czas od ostatniej aktualizacji frames_per_sec
	float						lag;			//opóŸnienie wzglêdem normalego tempa updatowania sceny
	unsigned int				frames;
	float						frames_per_sec;	//najwa¿niejszy licznik w grze

	bool						pause;

	//queue
	std::queue<Event*>*			events_queue;	//wskaŸnik na kolejke komunikatów w celu szybszego dostêpu

protected:
	~engine_interface(){};

	//FableEngine - ta klasa jest dostêpna bezpoœrednio z GamePlay

public:


};