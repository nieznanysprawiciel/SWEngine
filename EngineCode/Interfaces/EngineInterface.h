#pragma once

#include <queue>
#include "..\Engine\DX11_interfaces_container.h"

/*Taki interfejs b�dzie widzia� u�ytkownik silnika z wn�trza swoich klas odpowiedzialnych za fabu��.
 *U�ytkownik musi mie� ograniczone mo�liwo�ci, �eby przypadkiem nie namiesza�. Szczeg�lnie nie powinien
 *mie� mo�liwo�ci wysy�ania event�w, bo mog�oby to doprowadzi� do zap�tlenia.*/


class ControllersEngine;
struct IndexPrediction;
class Event;


class engine_interface : public DX11_constant_buffers_container
{
	// Dziedziczymy z DX11_constant_buffers_container a nie z DX11_interfaces_container, poniewa�
	// musimy wywo�a� funkcj� release_DirectX z klasy pochodnej. Inaczej nie zwolnimy tamtych obiekt�w wog�le.

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
	bool						directX_ready;			// je�eli zmienna jest niepoprawna, nie renderujemy

	bool						full_screen;
	HWND						window_handler;
	HINSTANCE					instance_handler;
	int							window_width;
	int							window_height;

	//Engines
	ControllersEngine*			controllers_engine;		//kontroluje AI (klawiatur� trzeba przenie��)
	MovementEngine*				movement_engine;		//przelicza pozycj� obiekt�w w nast�pnej klatce
	DisplayEngine*				display_engine;			//wy�wietla obiekty na scenie
	CollisionEngine*			collision_engine;		//liczy kolizje
	PhysicEngine*				physic_engine;			//liczy oddzia�ywania fizyczne
	ModelsManager*				models_manager;			//zarz�dza modelami, teksturami i materia�ami
	FableEngine*				fable_engine;			//zarz�dza fabu�� gry, interakcjami obiekt�w itd. Odpowiada za tre��
	SoundEngine*				sound_engine;			//zarz�dza muzyk� i d�wi�kami
	UI_Engine*					ui_engine;				//interfejs u�ytkownika (tak�e graficzny)

	//Objects
	//std::vector<IndexPrediction>		index_predictor;

	//time control
	__int64						timer_frequency;//cz�stotliwo�� timera
	__int64						time_previous;	//b�dziemy mierzy� r�nic� miedzy czasami w kolejnych
	__int64						elapsed_time;	//do licznika FPS�w, czas od ostatniej aktualizacji frames_per_sec
	float						lag;			//op�nienie wzgl�dem normalego tempa updatowania sceny
	unsigned int				frames;
	float						frames_per_sec;	//najwa�niejszy licznik w grze

	bool						pause;

	//queue
	std::queue<Event*>*			events_queue;	//wska�nik na kolejke komunikat�w w celu szybszego dost�pu

protected:
	~engine_interface(){};

	//FableEngine - ta klasa jest dost�pna bezpo�rednio z GamePlay

public:


};