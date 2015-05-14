#pragma once

/**@file InputabstractionLayer.h
@brief Plik zawiera deklaracjê klas InputAbstractionLayer_base, InputAbstractionLayer
struktur input_mapping i event_mapping oraz definicje standardowych warstw abstrakcji. */


#include "stdafx.h"


class Engine;

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

/**@brief Klasa reprezentuj¹ca warstwê abstrakcji miêdzy wartoœciami zwracanymi z directInputa, a wejœciem aplikacji.

W grze mo¿e istnieæ wiele wartstw abstrakcji, z których w danym momencie aktywna jest tylko jedna.
Warstwa abstrakcji umo¿liwia stworzenie zestawu w³asnych przycisków oraz kontrolerów posiadaj¹cych osie,
do których nastêpnie zostan¹ przypisane odpowiednie przyciski fizyczne. Wiele przycisków mo¿e jednoczeœnie
byæ mapowanych na jeden przycisk wirtualny.

InputAbstractionLayer_base odpowiada za interakcjê z kontrolerami i zapewnia ograniczony dostêp do zasobów
klasy, a co najwa¿niejsze, nie pozwala na edycjê danych.
Klasa InputAbstractionLayer s³u¿y jako interfejs do komunikacji dla obiektu UI_Engine.
Umo¿liwia modyfikowanie zawartoœci tej klasy. Zostaje w ka¿dej klatce uzupe³niona danymi,
które bêd¹ potem widziane przez kontrolery.

Do klasy bazowej nie nale¿y dodawaæ ¿adnych funkcji, umo¿liwiaj¹cych modyfikacjê danych o wejœciu.

U¿ytkownik tworz¹cy warstwê abstrakcji powinien stworzyæ obiekt klasy pochodnej, poniewa¿ tylko
on umo¿liwia zdefiniowanie takiej warstwy (czyli zdefiniowanie mapowania przycisków oraz podanie,
które z nich maj¹ wysy³aæ eventy przy wciœniêciu lub puszczeniu). Aby przeczytaæ wiêcej o wartwach
abstrakcji: @ref Input.

Kontrolery dostaj¹ wskaŸnik na warstwê abstrakcji, z któr¹ maj¹ wspó³pracowaæ. Aby odczytaæ dane,
nale¿y wywo³aæ funkcje get_buttons_table oraz get_axis_table. Zwracaj¹ one wskaŸnik na tablice,
pierwsza z przyciskami, a druga z po³o¿eniem na osi.
Przyciski s¹ opisywane zmiennymi typu char, gdzie wartoœæ 1 oznacza, ¿e przycisk jest wciœniêty,
a 0, ¿e nie jest. Kolejne przyciski znajduj¹ siê pod kolejnymi indeksami w tablicy. Zasadniczo nie
mo¿na siê dowiedzieæ, jakie jest znaczenie poszczególnych pól w tablicy, poniewa¿ wartwa abstrakcji
jest definiowana przez u¿ytkownika, który nastêpnie pisze dopasowany do tej wartwy kontroler. Wyj¹tek
stanowi¹ predefiniowane warstwy abstrakcji, które s¹ opisane w tej dokumentacji.
{do zrobienia}

Po³o¿enie na osi opisuje wychylenie jakiegoœ urz¹dzenia, np. joysticka. Jego wartoœæ jest typu
float i zawiera siê w przedziale [-1;1].

@Attention Ka¿dy kontroler przed pobraniem danych z tablic powinien siê zorientowaæ, czy warstwa
abstrakcji, na której pracuje jest aktywna. Robi siê to wywo³uj¹c funkcjê is_active. Kontrolery
powinny sterowaæ przypisanymi im obiektami jednie wtedy, gdy ich wartwa jest aktywna. W przeciwnym
razie bêd¹ odczytywaæ dane, które s¹ od dawana nieaktualne, poniewa¿ aktualizowana jest tylko aktywna warstwa.

Ponadto kontrolery nie powinny byæ przypisywane do niekompatybilnych z nimi wartw abstrakcji.
W szczególnoœci wypada³oby przynajmniej sprawdziæ czy zakres tablic wirtualnych jest wiêkszy, ni¿
wymagany przez kontroler.

*/
class InputAbstractionLayer_base
{
protected:
	short	mouseX;			///<Po³o¿enie myszki w X
	short	mouseY;			///<Po³o¿enie myszi w Y

	char*	virtual_button;	///<Tablica wirtualnych przycisków
	float*	virtual_axis;	///<Tablica dla kontrolerów osiowych

	unsigned short	num_buttons;	///<Liczba przycisków
	unsigned short	num_axis;		///<Liczba kontrolerów osiowych

	bool	active;			///<Stwierdza czy dana wartstwa abstrakcji jest aktualnie aktywna

	InputAbstractionLayer_base();
public:
	~InputAbstractionLayer_base();

	inline bool is_active() { return active; }	///<Informuje czy warstwa jest aktywna
	inline const char* get_buttons_table() { return virtual_button; }	///<Zwraca tablicê wirtualnych przycisków
	inline const float* get_axis_table() { return virtual_axis; }		///<Zwraca tablicê wirtualnych osi
	inline short get_mouseX() { return mouseX; }		///<Zwraca po³o¿enie myszy w X
	inline short get_mouseY() { return mouseY; }		///<Zwraca po³o¿enie myszy w Y
	inline unsigned short get_num_axis() { return num_axis; }			///<Zwraca liczbe osi
	inline unsigned short get_num_buttons() { return num_buttons; }	///<Zwraca liczbe przycisków
};

/**@brief Klasa stanowi interfejs umo¿liwiaj¹cy definiowanie i modyfikowanie wastwy abstrakcji.
Zalecam przeczytanie opisu klasy InputAbstractionLayer_base, który zawiera wszystkie potrzebne informacje.

Klasa bazowa stanowi interfejs uniemo¿liwiaj¹cy nadpisanie danych w klasie i tylko
w takim celu istnieje to rozdzielenie. Nie powinny istnieæ ¿adne obiekty klasy
InputAbstractionLayer_base, poniewa¿ nie ma to ¿adnego sensu. Taki obiekt nie móg³by
zostaæ w ¿aden sposób uzupe³niony danymi.
*/
class InputAbstractionLayer : public InputAbstractionLayer_base
{
private:
	/**Zmienna active_changed jest ustawiana w momencie, kiedy obiekt staje
		siê aktywn¹ warstw¹ abstrakcji.Ma za zadanie zapobiegac wysy³aniu eventów
		o wciœniêciu przycisków, które zosta³y wciœniête w momencie aktywnoœci poprzedniej
		wartwy abstrakcji.Zmienna chroni tylko przed eventami typu wciœniêcie przycisku,
		nie da siê za jej pomoc¹ zablokowaæ eventów typu puszczenie przycisku,
		poniewa¿ mo¿e to nast¹piæ wiele klatek póŸniej.*/
	bool active_changed;

	std::vector<input_mapping>		buttons_mapping_table;	///<Tablica mapowania przycisków fizycznych na wirtualne
	std::vector<input_mapping>		axis_mapping_table;		///<Tablica mapowania osi fizycznych na wirtualne
	std::vector<event_mapping>		event_mapping_table;	///<Tablica rz¹danych eventów

public:
	InputAbstractionLayer();
	~InputAbstractionLayer();

	void inline set_active(bool activate) { active = activate; if(active) active_changed = true; }	///<Ustawia warstwê abstrakcji jako aktywn¹
	void inline set_mouse_position(short X, short Y) { mouseX = X; mouseY = Y; }		///<Ustawia pozycjê myszy

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

W przypadku modyfikacji zmieniæ te¿ sta³¹ STANDARD_ABSTRACTION_LAYER_COUNT
*/
typedef enum
{
	PROTOTYPE_BUTTONS
} STANDARD_ABSTRACTION_LAYER;

/*Trzeba pamiêtaæ, ¿eby aktualizowac tê zmienn¹. Je¿eli siê tego nie zrobi,
grozi to usuniêciem standardowej wartwy abstrakcji z silnika.*/
#define STANDARD_ABSTRACTION_LAYER_COUNT		1

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
