#pragma once

#include "..\..\stdafx.h"


class Engine;

/*Struktura opisuj¹ca, które przyciski fizyczne
powinny zostaæ zmapowane na przyciski wirtualne.
device_nr		-	urz¹dzeniem, które mapujemy
virtual_index	-	index w tablicy wirtualnych przycisków (osi)
button_axis_id	-	identyfikator przycisku lub osi na urz¹dzeniu fizycznym*/
struct input_mapping
{
	short			device_nr;
	unsigned short	virtual_index;
	unsigned int	button_axis_id;
};


/*Struktura przechowuj¹ca informacje o przyciskach,
których wciœniêcie lub puszczenie ma powodowaæ wys³anie
eventu.
Informacja ta przechowywana jest w plach bitowych push_event i pull event.

Roboczo przechowywana jest wartoœc value.*/
struct event_mapping
{
	unsigned short	virtual_index;
	char	down_event : 4;
	char	up_event : 4;
	char	value;
};

/*klasa reprezentuj¹ca warstwê abstrakcji miêdzy wartoœciami zwracanymi
z directInputa, a wejœciem aplikacji. W grze mo¿e istnieæ wiele wartstw
abstrakcji, z których w danym momencie aktywna jest tylko jedna.

Warstwa abstrakcji umo¿liwia stworzenie zestawu w³asnych przycisków oraz
kontrolerów posiadaj¹cych osie, do których nastêpnie zostan¹ przypisane
odpowiednie przyciski fizyczne.

InputAbstractionLayer_base odpowiada za interakcjê z kontrolerami
i zapewnia ograniczony dostêp do zasobów klasy.

Klasa InputAbstractionLayer s³u¿y jako interfejs do komunikacji obiektem UI_Engine.
Umo¿liwia modyfikowanie zawartoœci tej klasy.

Do klasy bazowej nie nale¿y dodawaæ ¿adnych funkcji, umo¿liwiaj¹cych modyfikacjê
danych o wejœciu.

U¿ytkownik tworz¹cy warstwê abstrakcji powinien stworzyæ obiekt klasy pochodnej,
poniewa¿ tylko on umo¿liwia zdefiniowanie takiej warstwy.*/

class InputAbstractionLayer_base
{
protected:
	short	mouseX;
	short	mouseY;

	char*	virtual_button;
	float*	virtual_axis;

	unsigned short	num_buttons;
	unsigned short	num_axis;

	bool	active;

	InputAbstractionLayer_base();
public:
	~InputAbstractionLayer_base();

	inline bool is_active() { return active; };
	inline const char* get_buttons_table() { return virtual_button; };
	inline const float* get_axis_table() { return virtual_axis; };
	inline short get_mouseX() { return mouseX; };
	inline short get_mouseY() { return mouseY; };
	inline unsigned short get_num_axis() { return num_axis; };
	inline unsigned short get_num_buttons() { return num_buttons; };
};

/*Zmienna active_changed jest ustawiana w momencie, kiedy obiekt staje
siê aktywn¹ warstw¹ abstrakcji. Ma za zadanie zapobiegac wysy³aniu eventów
o wciœniêciu przycisków, które zosta³y wciœniête w momencie aktywnoœci poprzedniej
wartwy abstrakcji. Zmienna chroni tylko przed eventami typu wciœniêcie przycisku,
nie da siê za jej pomoc¹ zablokowaæ eventów typu puszczenie przycisku,
poniewa¿ mo¿e to nast¹piæ wiele klatek póŸniej.*/

class InputAbstractionLayer : public InputAbstractionLayer_base
{
private:
	bool active_changed;

	std::vector<input_mapping>		buttons_mapping_table;
	std::vector<input_mapping>		axis_mapping_table;
	std::vector<event_mapping>		event_mapping_table;

public:
	InputAbstractionLayer();
	~InputAbstractionLayer();

	void inline set_active(bool activate) { active = activate; if(active) active_changed = true; };
	void inline set_mouse_position(short X, short Y) { mouseX = X; mouseY = Y; };

	void begin_event_collection();
	void send_events(Engine*);

	void update_keyboard_device( short device_nr, const char* keyboard_state );
	void update_mouse_device( short device_nr, const DIMOUSESTATE2* mouse_state );
	void update_joystick_device( short device_nr, const DIJOYSTATE* joystick_state );

	void demand_down_event( unsigned short v_index );
	void demand_up_event( unsigned short v_index );
	void delete_up_event( unsigned short v_index );
	void delete_down_event( unsigned short v_index );

	void setup_buttons_layer( input_mapping* mapping, unsigned int length );
	void setup_axis_layer( input_mapping* mapping, unsigned int length );
	int setup_layer_from_file( const std::string& file_name );
};


//w przypadku modyfikacji zmieniæ te¿ sta³¹ STANDARD_ABSTRACTION_LAYER_COUNT
typedef enum
{
	PROTOTYPE
} STANDARD_ABSTRACTION_LAYER;

/*Trzeba pamiêtaæ, ¿eby aktualizowac tê zmienn¹. Je¿eli siê tego nie zrobi,
grozi to usuniêciem standardowej wartwy abstrakcji z silnika.*/
#define STANDARD_ABSTRACTION_LAYER_COUNT		1

typedef enum
{
	KEYBOARD,
	MOUSE,
	JOYSTICK
} DEVICE_IDs;


namespace STANDARD_LAYERS
{
//STANDARD_ABSTRACTION_LAYER::PROTOTYPE
	typedef enum
	{
		FORWARD,
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
		ESCAPE
	} PROTOTYPE;


#define PROTOTYPE_MAPPING_COUNT		11
extern input_mapping PROTOTYPE_mapping[PROTOTYPE_MAPPING_COUNT];


}
