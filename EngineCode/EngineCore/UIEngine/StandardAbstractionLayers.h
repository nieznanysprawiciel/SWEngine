#pragma once

#include "swCommonLib/Common/TypesDefinitions.h"
#include "EngineCore/EventsManager/Signal.h"


#include <vector>

typedef int16 DeviceNumber;
typedef uint16 VirtualKeyIndex;
typedef uint32 PhysicalKeyIndex;


/**Struktura opisuj�ca, kt�re przyciski fizyczne
powinny zosta� zmapowane na przyciski wirtualne.
DeviceNr		-	urz�dzenie, kt�re mapujemy
VirtualIndex	-	index w tablicy wirtualnych przycisk�w (osi)
PhysicalIndex	-	identyfikator przycisku lub osi na urz�dzeniu fizycznym*/
struct InputMapping
{
	DeviceNumber		DeviceNr;		///<Typ urz�dzenia jedna z warto�ci @ref DEVICE_IDs
	VirtualKeyIndex		VirtualIndex;	///<Index w tablicy wirtualnych przycisk�w (osi)
	PhysicalKeyIndex	PhysicalIndex;	///<Identyfikator przycisku lub osi na urz�dzeniu fizycznym
};


/**Struktura przechowuj�ca informacje o przyciskach,
kt�rych wci�ni�cie lub puszczenie ma powodowa� wys�anie
eventu.
Informacja ta przechowywana jest w polach bitowych push_event i pull event.

Roboczo przechowywana jest warto�� value.*/
struct EventMapping
{
	VirtualKeyIndex		VirtualIndex;				///<Indeks przycisku wirtualnego
	char				DownEvent : 4;				///<Info czy chcemy event o wci�ni�ciu
	char				UpEvent : 4;				///<Info czy chcemy event o puszczeniu
	char				Value;						///<Zmienna pomocnicza, nie u�ywa�
	EventListener		DownEventListener;			///< Do kogo wys�a� event o wci�ni�ciu przycisku.
	EventListener		UpEventListener;			///< Do kogo wys�a� event o puszczeniu przycisku.
};


/**@brief Enumeruje wbudowane wartstwy abstrakcji.

Sta�a STANDARD_ABSTRACTION_LAYER_COUNT oznacza ilo�� warstw abstrakcji.
Musi by� ostatnia na li�cie.
*/
enum STANDARD_ABSTRACTION_LAYER
{
	PROTOTYPE_BUTTONS,
	STANDARD_ABSTRACTION_LAYER_COUNT
};

/*Trzeba pami�ta�, �eby aktualizowac t� zmienn�. Je�eli si� tego nie zrobi,
grozi to usuni�ciem standardowej wartwy abstrakcji z silnika.*/
//#define STANDARD_ABSTRACTION_LAYER_COUNT		1

/**@brief Typy urzadze�, kt�re mo�emy mapowa� na przyciski/osie wirtualne.*/
enum DEVICE_IDs
{
	KEYBOARD,		///<Klawiatura
	MOUSE,			///<Mysz
	JOYSTICK		///<Joystick i wszystkie urz�dzenia, kt�rych si� nie da zaklasyfikowa� do myszy i klawiatury
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

