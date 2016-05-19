#pragma once

/**@file InputabstractionLayer.h
@brief Plik zawiera deklaracjê klas InputAbstractionLayerBase, InputAbstractionLayer
struktur InputMapping i EventMapping oraz definicje standardowych warstw abstrakcji. */


#include "EngineCore/UIEngine/StandardAbstractionLayers.h"
#include "EngineCore/UIEngine/IncludeDirectInput.h"		///@todo Wydzieliæ direct input do jakiegoœ modu³u i ukryæ implementacje, ¿eby nie by³o takich includów.

class Engine;


/**@brief Klasa reprezentuj¹ca warstwê abstrakcji miêdzy wartoœciami zwracanymi z directInputa, a wejœciem aplikacji.

W grze mo¿e istnieæ wiele wartstw abstrakcji, z których w danym momencie aktywna jest tylko jedna.
Warstwa abstrakcji umo¿liwia stworzenie zestawu w³asnych przycisków oraz kontrolerów posiadaj¹cych osie,
do których nastêpnie zostan¹ przypisane odpowiednie przyciski fizyczne. Wiele przycisków mo¿e jednoczeœnie
byæ mapowanych na jeden przycisk wirtualny.

InputAbstractionLayerBase odpowiada za interakcjê z kontrolerami i zapewnia ograniczony dostêp do zasobów
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
nale¿y wywo³aæ funkcje GetButtonsTable oraz GetAxisTable. Zwracaj¹ one wskaŸnik na tablice,
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
abstrakcji, na której pracuje jest aktywna. Robi siê to wywo³uj¹c funkcjê IsActive. Kontrolery
powinny sterowaæ przypisanymi im obiektami jednie wtedy, gdy ich wartwa jest aktywna. W przeciwnym
razie bêd¹ odczytywaæ dane, które s¹ od dawana nieaktualne, poniewa¿ aktualizowana jest tylko aktywna warstwa.

Ponadto kontrolery nie powinny byæ przypisywane do niekompatybilnych z nimi wartw abstrakcji.
W szczególnoœci wypada³oby przynajmniej sprawdziæ czy zakres tablic wirtualnych jest wiêkszy, ni¿
wymagany przez kontroler.

*/
class InputAbstractionLayerBase
{
protected:
	short	mouseX;			///<Po³o¿enie myszki w X
	short	mouseY;			///<Po³o¿enie myszi w Y

	std::vector< char >		m_virtualButtons;		///<Tablica wirtualnych przycisków
	std::vector< float >	m_virtualAxis;			///<Tablica dla kontrolerów osiowych

	bool	active;			///<Stwierdza czy dana wartstwa abstrakcji jest aktualnie aktywna

	InputAbstractionLayerBase();
public:
	~InputAbstractionLayerBase();

	inline bool				IsActive()				{ return active; }						///<Informuje czy warstwa jest aktywna
	inline short			GetMouseX()				{ return mouseX; }						///<Zwraca po³o¿enie myszy w X
	inline short			GetMouseY()				{ return mouseY; }						///<Zwraca po³o¿enie myszy w Y
	inline unsigned short	GetNumAxis()			{ return (unsigned short)m_virtualAxis.size(); }		///<Zwraca liczbe osi
	inline unsigned short	GetNumButtons()			{ return (unsigned short)m_virtualButtons.size(); }		///<Zwraca liczbe przycisków

	inline const std::vector< char >&		GetButtonsTable()		{ return m_virtualButtons; }	///<Zwraca tablicê wirtualnych przycisków
	inline const std::vector< float >&		GetAxisTable()			{ return m_virtualAxis; }	///<Zwraca tablicê wirtualnych osi
};

/**@brief Klasa stanowi interfejs umo¿liwiaj¹cy definiowanie i modyfikowanie wastwy abstrakcji.
Zalecam przeczytanie opisu klasy InputAbstractionLayerBase, który zawiera wszystkie potrzebne informacje.

Klasa bazowa stanowi interfejs uniemo¿liwiaj¹cy nadpisanie danych w klasie i tylko
w takim celu istnieje to rozdzielenie. Nie powinny istnieæ ¿adne obiekty klasy
InputAbstractionLayerBase, poniewa¿ nie ma to ¿adnego sensu. Taki obiekt nie móg³by
zostaæ w ¿aden sposób uzupe³niony danymi.
*/
class InputAbstractionLayer : public InputAbstractionLayerBase
{
private:
	/**Zmienna m_activeChanged jest ustawiana w momencie, kiedy obiekt staje
		siê aktywn¹ warstw¹ abstrakcji.Ma za zadanie zapobiegac wysy³aniu eventów
		o wciœniêciu przycisków, które zosta³y wciœniête w momencie aktywnoœci poprzedniej
		wartwy abstrakcji.Zmienna chroni tylko przed eventami typu wciœniêcie przycisku,
		nie da siê za jej pomoc¹ zablokowaæ eventów typu puszczenie przycisku,
		poniewa¿ mo¿e to nast¹piæ wiele klatek póŸniej.*/
	bool m_activeChanged;

	std::vector<InputMapping>		m_buttonsMapping;	///<Tablica mapowania przycisków fizycznych na wirtualne
	std::vector<InputMapping>		m_axisMapping;		///<Tablica mapowania osi fizycznych na wirtualne
	std::vector<EventMapping>		m_requestedEvents;	///<Tablica ¿¹danych eventów

public:
	InputAbstractionLayer();
	~InputAbstractionLayer();

	void inline set_active			( bool activate ) { active = activate; if( active ) m_activeChanged = true; }	///<Ustawia warstwê abstrakcji jako aktywn¹
	void inline set_mouse_position	( short X, short Y ) { mouseX = X; mouseY = Y; }								///<Ustawia pozycjê myszy

	void	begin_event_collection		();
	void	send_events					(Engine*);

	void	update_keyboard_device		( DeviceNumber DeviceNr, const char* keyboard_state );
	void	update_mouse_device			( DeviceNumber DeviceNr, const DIMOUSESTATE2* mouse_state, int window_width, int window_height);
	void	update_joystick_device		( DeviceNumber DeviceNr, const DIJOYSTATE* joystick_state );

	void	demand_down_event			( VirtualKeyIndex virtualIdx );
	void	demand_up_event				( VirtualKeyIndex virtualIdx );
	void	delete_up_event				( VirtualKeyIndex virtualIdx );
	void	delete_down_event			( VirtualKeyIndex virtualIdx );

	void	SetupButtonsLayer			( std::vector< InputMapping >&& mapping );
	void	SetupAxisLayer				( std::vector< InputMapping >&& mapping );
	int		SetupLayerFromFile			( const std::string& file_name );
};

