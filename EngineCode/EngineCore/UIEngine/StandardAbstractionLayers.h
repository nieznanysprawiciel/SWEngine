#pragma once

#include "swCommonLib/Common/TypesDefinitions.h"
#include "EngineCore/EventsManager/Signal.h"


#include <vector>

typedef int16 DeviceNumber;
typedef uint16 VirtualKeyIndex;
typedef uint32 PhysicalKeyIndex;


/**Struktura opisuj¹ca, które przyciski fizyczne
powinny zostaæ zmapowane na przyciski wirtualne.
DeviceNr		-	urz¹dzenie, które mapujemy
VirtualIndex	-	index w tablicy wirtualnych przycisków (osi)
PhysicalIndex	-	identyfikator przycisku lub osi na urz¹dzeniu fizycznym*/
struct InputMapping
{
	DeviceNumber		DeviceNr;		///<Typ urz¹dzenia jedna z wartoœci @ref DEVICE_IDs
	VirtualKeyIndex		VirtualIndex;	///<Index w tablicy wirtualnych przycisków (osi)
	PhysicalKeyIndex	PhysicalIndex;	///<Identyfikator przycisku lub osi na urz¹dzeniu fizycznym
};


/**Struktura przechowuj¹ca informacje o przyciskach,
których wciœniêcie lub puszczenie ma powodowaæ wys³anie
eventu.
Informacja ta przechowywana jest w polach bitowych push_event i pull event.

Roboczo przechowywana jest wartoœæ value.*/
struct EventMapping
{
	VirtualKeyIndex		VirtualIndex;				///<Indeks przycisku wirtualnego
	char				DownEvent : 4;				///<Info czy chcemy event o wciœniêciu
	char				UpEvent : 4;				///<Info czy chcemy event o puszczeniu
	char				Value;						///<Zmienna pomocnicza, nie u¿ywaæ
	EventListener		DownEventListener;			///< Do kogo wys³aæ event o wciœniêciu przycisku.
	EventListener		UpEventListener;			///< Do kogo wys³aæ event o puszczeniu przycisku.
};


/**@brief Enumeruje wbudowane wartstwy abstrakcji.

Sta³a STANDARD_ABSTRACTION_LAYER_COUNT oznacza iloœæ warstw abstrakcji.
Musi byæ ostatnia na liœcie.
*/
enum STANDARD_ABSTRACTION_LAYER
{
	PROTOTYPE_BUTTONS,
	STANDARD_ABSTRACTION_LAYER_COUNT
};

/*Trzeba pamiêtaæ, ¿eby aktualizowac tê zmienn¹. Je¿eli siê tego nie zrobi,
grozi to usuniêciem standardowej wartwy abstrakcji z silnika.*/
//#define STANDARD_ABSTRACTION_LAYER_COUNT		1

/**@brief Typy urzadzeñ, które mo¿emy mapowaæ na przyciski/osie wirtualne.*/
enum DEVICE_IDs
{
	KEYBOARD,		///<Klawiatura
	MOUSE,			///<Mysz
	JOYSTICK		///<Joystick i wszystkie urz¹dzenia, których siê nie da zaklasyfikowaæ do myszy i klawiatury
};



namespace STANDARD_LAYERS
{
//STANDARD_ABSTRACTION_LAYER::PROTOTYPE_BUTTONS
	typedef enum
	{
		FORWARD = 0,
		BACKWARD,
		LEFT,
		RIGHT,
		UP,
		DOWN,
		TURN_LEFT,
		TURN_RIGHT,
		TURN_UP,
		TURN_DOWN,
		SHOT1,
		SHOT2,
		SHOT3,
		MENU,
		PAUSE,
		ESCAPE,
		RIGHT_CLICK,
		LEFT_CLICK,
		GENERATE_LIGHTMAPS1,
		GENERATE_LIGHTMAPS2,
		GENERATE_LIGHTMAPS3,
		GENERATE_LIGHTMAPS4,
		GENERATE_LIGHTMAPS5,
		LOAD_LIGHTMAP_SCENE,
		ENABLE_MOVE_X,
		ENABLE_MOVE_Y,
		ENABLE_MOVE_Z,

		PROTOTYPE_BUTTONS_MAPPING_COUNT
	} PROTOTYPE_BUTTONS;

	//#define PROTOTYPE_BUTTONS_MAPPING_COUNT		11
	extern std::vector< InputMapping > PROTOTYPE_BUTTONS_mapping;

	typedef enum
	{
		X_AXIS = 0,
		Y_AXIS,
		ZOOM,

		PROTOTYPE_AXES_MAPPING_COUNT
	} PROTOTYPE_AXES;

	//#define PROTOTYPE_AXES_MAPPING_COUNT		3
	extern std::vector< InputMapping > PROTOTYPE_AXES_mapping;

}

