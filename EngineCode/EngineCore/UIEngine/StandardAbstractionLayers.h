#pragma once

/**Struktura opisuj¹ca, które przyciski fizyczne
powinny zostaæ zmapowane na przyciski wirtualne.
device_nr		-	urz¹dzeniem, które mapujemy
virtual_index	-	index w tablicy wirtualnych przycisków (osi)
button_axis_id	-	identyfikator przycisku lub osi na urz¹dzeniu fizycznym*/
struct input_mapping
{
	short			device_nr;		///<Typ urz¹dzenia jedna z wartoœci @ref DEVICE_IDs
	unsigned short	virtual_index;	///<Index w tablicy wirtualnych przycisków (osi)
	unsigned int	button_axis_id;	///<Identyfikator przycisku lub osi na urz¹dzeniu fizycznym
};


/**Struktura przechowuj¹ca informacje o przyciskach,
których wciœniêcie lub puszczenie ma powodowaæ wys³anie
eventu.
Informacja ta przechowywana jest w polach bitowych push_event i pull event.

Roboczo przechowywana jest wartoœæ value.*/
struct event_mapping
{
	unsigned short	virtual_index;		///<Indeks przycisku wirtualnego
	char	down_event : 4;				///<Info czy chcemy event o wciœniêciu
	char	up_event : 4;				///<Info czy chcemy event o puszczeniu
	char	value;						///<Zmienna pomocnicza, nie u¿ywaæ
};


/**@brief Enumeruje wbudowane wartstwy abstrakcji.

Sta³a STANDARD_ABSTRACTION_LAYER_COUNT oznacza iloœæ warstw abstrakcji.
Musi byæ ostatnia na liœcie.
*/
typedef enum
{
	PROTOTYPE_BUTTONS,
	STANDARD_ABSTRACTION_LAYER_COUNT
} STANDARD_ABSTRACTION_LAYER;

/*Trzeba pamiêtaæ, ¿eby aktualizowac tê zmienn¹. Je¿eli siê tego nie zrobi,
grozi to usuniêciem standardowej wartwy abstrakcji z silnika.*/
//#define STANDARD_ABSTRACTION_LAYER_COUNT		1

/**@brief Typy urzadzeñ, które mo¿emy mapowaæ na przyciski/osie wirtualne.*/
typedef enum
{
	KEYBOARD,		///<Klawiatura
	MOUSE,			///<Mysz
	JOYSTICK		///<Joystick i wszystkie urz¹dzenia, których siê nie da zaklasyfikowaæ do myszy i klawiatury
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

