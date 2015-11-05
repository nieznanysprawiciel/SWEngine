#pragma once

/**Struktura opisuj�ca, kt�re przyciski fizyczne
powinny zosta� zmapowane na przyciski wirtualne.
device_nr		-	urz�dzeniem, kt�re mapujemy
virtual_index	-	index w tablicy wirtualnych przycisk�w (osi)
button_axis_id	-	identyfikator przycisku lub osi na urz�dzeniu fizycznym*/
struct input_mapping
{
	short			device_nr;		///<Typ urz�dzenia jedna z warto�ci @ref DEVICE_IDs
	unsigned short	virtual_index;	///<Index w tablicy wirtualnych przycisk�w (osi)
	unsigned int	button_axis_id;	///<Identyfikator przycisku lub osi na urz�dzeniu fizycznym
};


/**Struktura przechowuj�ca informacje o przyciskach,
kt�rych wci�ni�cie lub puszczenie ma powodowa� wys�anie
eventu.
Informacja ta przechowywana jest w polach bitowych push_event i pull event.

Roboczo przechowywana jest warto�� value.*/
struct event_mapping
{
	unsigned short	virtual_index;		///<Indeks przycisku wirtualnego
	char	down_event : 4;				///<Info czy chcemy event o wci�ni�ciu
	char	up_event : 4;				///<Info czy chcemy event o puszczeniu
	char	value;						///<Zmienna pomocnicza, nie u�ywa�
};


/**@brief Enumeruje wbudowane wartstwy abstrakcji.

Sta�a STANDARD_ABSTRACTION_LAYER_COUNT oznacza ilo�� warstw abstrakcji.
Musi by� ostatnia na li�cie.
*/
typedef enum
{
	PROTOTYPE_BUTTONS,
	STANDARD_ABSTRACTION_LAYER_COUNT
} STANDARD_ABSTRACTION_LAYER;

/*Trzeba pami�ta�, �eby aktualizowac t� zmienn�. Je�eli si� tego nie zrobi,
grozi to usuni�ciem standardowej wartwy abstrakcji z silnika.*/
//#define STANDARD_ABSTRACTION_LAYER_COUNT		1

/**@brief Typy urzadze�, kt�re mo�emy mapowa� na przyciski/osie wirtualne.*/
typedef enum
{
	KEYBOARD,		///<Klawiatura
	MOUSE,			///<Mysz
	JOYSTICK		///<Joystick i wszystkie urz�dzenia, kt�rych si� nie da zaklasyfikowa� do myszy i klawiatury
} DEVICE_IDs;



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
		SHOT1,
		SHOT2,
		SHOT3,
		MENU,
		PAUSE,
		ESCAPE,
		GENERATE_LIGHTMAPS,
		PROTOTYPE_BUTTONS_MAPPING_COUNT
	} PROTOTYPE_BUTTONS;

	//#define PROTOTYPE_BUTTONS_MAPPING_COUNT		11
	extern input_mapping PROTOTYPE_BUTTONS_mapping[ PROTOTYPE_BUTTONS_MAPPING_COUNT ];

	typedef enum
	{
		X_AXIS = 0,
		Y_AXIS, 
		Z_AXIS,
		PROTOTYPE_AXES_MAPPING_COUNT
	} PROTOTYPE_AXES;

	//#define PROTOTYPE_AXES_MAPPING_COUNT		3
	extern input_mapping PROTOTYPE_AXES_mapping[ PROTOTYPE_AXES_MAPPING_COUNT ];

}

