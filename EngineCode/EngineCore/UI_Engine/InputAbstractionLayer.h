#pragma once

/**@file InputabstractionLayer.h
@brief Plik zawiera deklaracj� klas InputAbstractionLayer_base, InputAbstractionLayer
struktur input_mapping i event_mapping oraz definicje standardowych warstw abstrakcji. */


#include "stdafx.h"


class Engine;

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

/**@brief Klasa reprezentuj�ca warstw� abstrakcji mi�dzy warto�ciami zwracanymi z directInputa, a wej�ciem aplikacji.

W grze mo�e istnie� wiele wartstw abstrakcji, z kt�rych w danym momencie aktywna jest tylko jedna.
Warstwa abstrakcji umo�liwia stworzenie zestawu w�asnych przycisk�w oraz kontroler�w posiadaj�cych osie,
do kt�rych nast�pnie zostan� przypisane odpowiednie przyciski fizyczne. Wiele przycisk�w mo�e jednocze�nie
by� mapowanych na jeden przycisk wirtualny.

InputAbstractionLayer_base odpowiada za interakcj� z kontrolerami i zapewnia ograniczony dost�p do zasob�w
klasy, a co najwa�niejsze, nie pozwala na edycj� danych.
Klasa InputAbstractionLayer s�u�y jako interfejs do komunikacji dla obiektu UI_Engine.
Umo�liwia modyfikowanie zawarto�ci tej klasy. Zostaje w ka�dej klatce uzupe�niona danymi,
kt�re b�d� potem widziane przez kontrolery.

Do klasy bazowej nie nale�y dodawa� �adnych funkcji, umo�liwiaj�cych modyfikacj� danych o wej�ciu.

U�ytkownik tworz�cy warstw� abstrakcji powinien stworzy� obiekt klasy pochodnej, poniewa� tylko
on umo�liwia zdefiniowanie takiej warstwy (czyli zdefiniowanie mapowania przycisk�w oraz podanie,
kt�re z nich maj� wysy�a� eventy przy wci�ni�ciu lub puszczeniu). Aby przeczyta� wi�cej o wartwach
abstrakcji: @ref Input.

Kontrolery dostaj� wska�nik na warstw� abstrakcji, z kt�r� maj� wsp�pracowa�. Aby odczyta� dane,
nale�y wywo�a� funkcje get_buttons_table oraz get_axis_table. Zwracaj� one wska�nik na tablice,
pierwsza z przyciskami, a druga z po�o�eniem na osi.
Przyciski s� opisywane zmiennymi typu char, gdzie warto�� 1 oznacza, �e przycisk jest wci�ni�ty,
a 0, �e nie jest. Kolejne przyciski znajduj� si� pod kolejnymi indeksami w tablicy. Zasadniczo nie
mo�na si� dowiedzie�, jakie jest znaczenie poszczeg�lnych p�l w tablicy, poniewa� wartwa abstrakcji
jest definiowana przez u�ytkownika, kt�ry nast�pnie pisze dopasowany do tej wartwy kontroler. Wyj�tek
stanowi� predefiniowane warstwy abstrakcji, kt�re s� opisane w tej dokumentacji.
{do zrobienia}

Po�o�enie na osi opisuje wychylenie jakiego� urz�dzenia, np. joysticka. Jego warto�� jest typu
float i zawiera si� w przedziale [-1;1].

@Attention Ka�dy kontroler przed pobraniem danych z tablic powinien si� zorientowa�, czy warstwa
abstrakcji, na kt�rej pracuje jest aktywna. Robi si� to wywo�uj�c funkcj� is_active. Kontrolery
powinny sterowa� przypisanymi im obiektami jednie wtedy, gdy ich wartwa jest aktywna. W przeciwnym
razie b�d� odczytywa� dane, kt�re s� od dawana nieaktualne, poniewa� aktualizowana jest tylko aktywna warstwa.

Ponadto kontrolery nie powinny by� przypisywane do niekompatybilnych z nimi wartw abstrakcji.
W szczeg�lno�ci wypada�oby przynajmniej sprawdzi� czy zakres tablic wirtualnych jest wi�kszy, ni�
wymagany przez kontroler.

*/
class InputAbstractionLayer_base
{
protected:
	short	mouseX;			///<Po�o�enie myszki w X
	short	mouseY;			///<Po�o�enie myszi w Y

	char*	virtual_button;	///<Tablica wirtualnych przycisk�w
	float*	virtual_axis;	///<Tablica dla kontroler�w osiowych

	unsigned short	num_buttons;	///<Liczba przycisk�w
	unsigned short	num_axis;		///<Liczba kontroler�w osiowych

	bool	active;			///<Stwierdza czy dana wartstwa abstrakcji jest aktualnie aktywna

	InputAbstractionLayer_base();
public:
	~InputAbstractionLayer_base();

	inline bool is_active() { return active; }	///<Informuje czy warstwa jest aktywna
	inline const char* get_buttons_table() { return virtual_button; }	///<Zwraca tablic� wirtualnych przycisk�w
	inline const float* get_axis_table() { return virtual_axis; }		///<Zwraca tablic� wirtualnych osi
	inline short get_mouseX() { return mouseX; }		///<Zwraca po�o�enie myszy w X
	inline short get_mouseY() { return mouseY; }		///<Zwraca po�o�enie myszy w Y
	inline unsigned short get_num_axis() { return num_axis; }			///<Zwraca liczbe osi
	inline unsigned short get_num_buttons() { return num_buttons; }	///<Zwraca liczbe przycisk�w
};

/**@brief Klasa stanowi interfejs umo�liwiaj�cy definiowanie i modyfikowanie wastwy abstrakcji.
Zalecam przeczytanie opisu klasy InputAbstractionLayer_base, kt�ry zawiera wszystkie potrzebne informacje.

Klasa bazowa stanowi interfejs uniemo�liwiaj�cy nadpisanie danych w klasie i tylko
w takim celu istnieje to rozdzielenie. Nie powinny istnie� �adne obiekty klasy
InputAbstractionLayer_base, poniewa� nie ma to �adnego sensu. Taki obiekt nie m�g�by
zosta� w �aden spos�b uzupe�niony danymi.
*/
class InputAbstractionLayer : public InputAbstractionLayer_base
{
private:
	/**Zmienna active_changed jest ustawiana w momencie, kiedy obiekt staje
		si� aktywn� warstw� abstrakcji.Ma za zadanie zapobiegac wysy�aniu event�w
		o wci�ni�ciu przycisk�w, kt�re zosta�y wci�ni�te w momencie aktywno�ci poprzedniej
		wartwy abstrakcji.Zmienna chroni tylko przed eventami typu wci�ni�cie przycisku,
		nie da si� za jej pomoc� zablokowa� event�w typu puszczenie przycisku,
		poniewa� mo�e to nast�pi� wiele klatek p�niej.*/
	bool active_changed;

	std::vector<input_mapping>		buttons_mapping_table;	///<Tablica mapowania przycisk�w fizycznych na wirtualne
	std::vector<input_mapping>		axis_mapping_table;		///<Tablica mapowania osi fizycznych na wirtualne
	std::vector<event_mapping>		event_mapping_table;	///<Tablica rz�danych event�w

public:
	InputAbstractionLayer();
	~InputAbstractionLayer();

	void inline set_active(bool activate) { active = activate; if(active) active_changed = true; }	///<Ustawia warstw� abstrakcji jako aktywn�
	void inline set_mouse_position(short X, short Y) { mouseX = X; mouseY = Y; }		///<Ustawia pozycj� myszy

	void begin_event_collection();
	void send_events(Engine*);

	void update_keyboard_device( short device_nr, const char* keyboard_state );
	void update_mouse_device( short device_nr, const DIMOUSESTATE2* mouse_state, int window_width, int window_height);
	void update_joystick_device( short device_nr, const DIJOYSTATE* joystick_state );

	void demand_down_event( unsigned short v_index );
	void demand_up_event( unsigned short v_index );
	void delete_up_event( unsigned short v_index );
	void delete_down_event( unsigned short v_index );

	void setup_buttons_layer( input_mapping* mapping, unsigned int length );
	void setup_axis_layer( input_mapping* mapping, unsigned int length );
	int setup_layer_from_file( const std::string& file_name );
};


/**@brief Enumeruje wbudowane wartstwy abstrakcji.

W przypadku modyfikacji zmieni� te� sta�� STANDARD_ABSTRACTION_LAYER_COUNT
*/
typedef enum
{
	PROTOTYPE_BUTTONS
} STANDARD_ABSTRACTION_LAYER;

/*Trzeba pami�ta�, �eby aktualizowac t� zmienn�. Je�eli si� tego nie zrobi,
grozi to usuni�ciem standardowej wartwy abstrakcji z silnika.*/
#define STANDARD_ABSTRACTION_LAYER_COUNT		1

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
	} PROTOTYPE_BUTTONS;

	#define PROTOTYPE_BUTTONS_MAPPING_COUNT		11
	extern input_mapping PROTOTYPE_BUTTONS_mapping[PROTOTYPE_BUTTONS_MAPPING_COUNT];

	typedef enum
	{
		X_AXIS,
		Y_AXIS, 
		Z_AXIS
	} PROTOTYPE_AXES;

	#define PROTOTYPE_AXES_MAPPING_COUNT		3
	extern input_mapping PROTOTYPE_AXES_mapping[PROTOTYPE_AXES_MAPPING_COUNT];

}
