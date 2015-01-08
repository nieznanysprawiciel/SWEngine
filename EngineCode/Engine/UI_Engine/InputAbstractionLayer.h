#pragma once

#include "..\..\stdafx.h"


class Engine;

/*Struktura opisuj�ca, kt�re przyciski fizyczne
powinny zosta� zmapowane na przyciski wirtualne.
device_nr		-	urz�dzeniem, kt�re mapujemy
virtual_index	-	index w tablicy wirtualnych przycisk�w (osi)
button_axis_id	-	identyfikator przycisku lub osi na urz�dzeniu fizycznym*/
struct input_mapping
{
	short			device_nr;
	unsigned short	virtual_index;
	unsigned int	button_axis_id;
};


/*Struktura przechowuj�ca informacje o przyciskach,
kt�rych wci�ni�cie lub puszczenie ma powodowa� wys�anie
eventu.
Informacja ta przechowywana jest w plach bitowych push_event i pull event.

Roboczo przechowywana jest warto�c value.*/
struct event_mapping
{
	unsigned short	virtual_index;
	char	down_event : 4;
	char	up_event : 4;
	char	value;
};

/*klasa reprezentuj�ca warstw� abstrakcji mi�dzy warto�ciami zwracanymi
z directInputa, a wej�ciem aplikacji. W grze mo�e istnie� wiele wartstw
abstrakcji, z kt�rych w danym momencie aktywna jest tylko jedna.

Warstwa abstrakcji umo�liwia stworzenie zestawu w�asnych przycisk�w oraz
kontroler�w posiadaj�cych osie, do kt�rych nast�pnie zostan� przypisane
odpowiednie przyciski fizyczne.

InputAbstractionLayer_base odpowiada za interakcj� z kontrolerami
i zapewnia ograniczony dost�p do zasob�w klasy.

Klasa InputAbstractionLayer s�u�y jako interfejs do komunikacji obiektem UI_Engine.
Umo�liwia modyfikowanie zawarto�ci tej klasy.

Do klasy bazowej nie nale�y dodawa� �adnych funkcji, umo�liwiaj�cych modyfikacj�
danych o wej�ciu.

U�ytkownik tworz�cy warstw� abstrakcji powinien stworzy� obiekt klasy pochodnej,
poniewa� tylko on umo�liwia zdefiniowanie takiej warstwy.*/

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
si� aktywn� warstw� abstrakcji. Ma za zadanie zapobiegac wysy�aniu event�w
o wci�ni�ciu przycisk�w, kt�re zosta�y wci�ni�te w momencie aktywno�ci poprzedniej
wartwy abstrakcji. Zmienna chroni tylko przed eventami typu wci�ni�cie przycisku,
nie da si� za jej pomoc� zablokowa� event�w typu puszczenie przycisku,
poniewa� mo�e to nast�pi� wiele klatek p�niej.*/

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


//w przypadku modyfikacji zmieni� te� sta�� STANDARD_ABSTRACTION_LAYER_COUNT
typedef enum
{
	PROTOTYPE
} STANDARD_ABSTRACTION_LAYER;

/*Trzeba pami�ta�, �eby aktualizowac t� zmienn�. Je�eli si� tego nie zrobi,
grozi to usuni�ciem standardowej wartwy abstrakcji z silnika.*/
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
