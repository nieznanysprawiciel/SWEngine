#pragma once

#include <queue>

/*Taki interfejs b�dzie widzia� u�ytkownik silnika z wn�trza swoich klas odpowiedzialnych za fabu��.
 *U�ytkownik musi mie� ograniczone mo�liwo�ci, �eby przypadkiem nie namiesza�. Szczeg�lnie nie powinien
 *mie� mo�liwo�ci wysy�ania event�w, bo mog�oby to doprowadzi� do zap�tlenia.*/


class ControllersEngine;
struct IndexPrediction;
class Event;
struct IDirect3DDevice9;
struct IDirect3D;
typedef struct IDirect3D9 *LPDIRECT3D9, *PDIRECT3D9;
typedef struct IDirect3DDevice9 *LPDIRECT3DDEVICE9, *PDIRECT3DDEVICE9;

class engine_interface
{
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
	LPDIRECT3D9					directX_interface;		// the pointer to our Direct3D interface
	LPDIRECT3DDEVICE9			directX_device;			// the pointer to the device class
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