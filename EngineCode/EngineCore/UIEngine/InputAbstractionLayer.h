#pragma once

/**@file InputabstractionLayer.h
@brief Plik zawiera deklaracj� klas InputAbstractionLayerBase, InputAbstractionLayer
struktur InputMapping i EventMapping oraz definicje standardowych warstw abstrakcji. */


#include "EngineCore/UIEngine/StandardAbstractionLayers.h"
#include "EngineCore/UIEngine/IncludeDirectInput.h"		///@todo Wydzieli� direct input do jakiego� modu�u i ukry� implementacje, �eby nie by�o takich includ�w.

class Engine;


/**@brief Klasa reprezentuj�ca warstw� abstrakcji mi�dzy warto�ciami zwracanymi z directInputa, a wej�ciem aplikacji.

W grze mo�e istnie� wiele wartstw abstrakcji, z kt�rych w danym momencie aktywna jest tylko jedna.
Warstwa abstrakcji umo�liwia stworzenie zestawu w�asnych przycisk�w oraz kontroler�w posiadaj�cych osie,
do kt�rych nast�pnie zostan� przypisane odpowiednie przyciski fizyczne. Wiele przycisk�w mo�e jednocze�nie
by� mapowanych na jeden przycisk wirtualny.

InputAbstractionLayerBase odpowiada za interakcj� z kontrolerami i zapewnia ograniczony dost�p do zasob�w
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
nale�y wywo�a� funkcje GetButtonsTable oraz GetAxisTable. Zwracaj� one wska�nik na tablice,
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
abstrakcji, na kt�rej pracuje jest aktywna. Robi si� to wywo�uj�c funkcj� IsActive. Kontrolery
powinny sterowa� przypisanymi im obiektami jednie wtedy, gdy ich wartwa jest aktywna. W przeciwnym
razie b�d� odczytywa� dane, kt�re s� od dawana nieaktualne, poniewa� aktualizowana jest tylko aktywna warstwa.

Ponadto kontrolery nie powinny by� przypisywane do niekompatybilnych z nimi wartw abstrakcji.
W szczeg�lno�ci wypada�oby przynajmniej sprawdzi� czy zakres tablic wirtualnych jest wi�kszy, ni�
wymagany przez kontroler.

*/
class InputAbstractionLayerBase
{
protected:
	short	mouseX;			///<Po�o�enie myszki w X
	short	mouseY;			///<Po�o�enie myszi w Y

	std::vector< char >		m_virtualButtons;		///<Tablica wirtualnych przycisk�w
	std::vector< float >	m_virtualAxis;			///<Tablica dla kontroler�w osiowych

	bool	active;			///<Stwierdza czy dana wartstwa abstrakcji jest aktualnie aktywna

	InputAbstractionLayerBase();
public:
	~InputAbstractionLayerBase();

	inline bool				IsActive()				{ return active; }						///<Informuje czy warstwa jest aktywna
	inline short			GetMouseX()				{ return mouseX; }						///<Zwraca po�o�enie myszy w X
	inline short			GetMouseY()				{ return mouseY; }						///<Zwraca po�o�enie myszy w Y
	inline unsigned short	GetNumAxis()			{ return (unsigned short)m_virtualAxis.size(); }		///<Zwraca liczbe osi
	inline unsigned short	GetNumButtons()			{ return (unsigned short)m_virtualButtons.size(); }		///<Zwraca liczbe przycisk�w

	inline const std::vector< char >&		GetButtonsTable()		{ return m_virtualButtons; }	///<Zwraca tablic� wirtualnych przycisk�w
	inline const std::vector< float >&		GetAxisTable()			{ return m_virtualAxis; }	///<Zwraca tablic� wirtualnych osi
};

/**@brief Klasa stanowi interfejs umo�liwiaj�cy definiowanie i modyfikowanie wastwy abstrakcji.
Zalecam przeczytanie opisu klasy InputAbstractionLayerBase, kt�ry zawiera wszystkie potrzebne informacje.

Klasa bazowa stanowi interfejs uniemo�liwiaj�cy nadpisanie danych w klasie i tylko
w takim celu istnieje to rozdzielenie. Nie powinny istnie� �adne obiekty klasy
InputAbstractionLayerBase, poniewa� nie ma to �adnego sensu. Taki obiekt nie m�g�by
zosta� w �aden spos�b uzupe�niony danymi.
*/
class InputAbstractionLayer : public InputAbstractionLayerBase
{
private:
	/**Zmienna m_activeChanged jest ustawiana w momencie, kiedy obiekt staje
		si� aktywn� warstw� abstrakcji.Ma za zadanie zapobiegac wysy�aniu event�w
		o wci�ni�ciu przycisk�w, kt�re zosta�y wci�ni�te w momencie aktywno�ci poprzedniej
		wartwy abstrakcji.Zmienna chroni tylko przed eventami typu wci�ni�cie przycisku,
		nie da si� za jej pomoc� zablokowa� event�w typu puszczenie przycisku,
		poniewa� mo�e to nast�pi� wiele klatek p�niej.*/
	bool m_activeChanged;

	std::vector<InputMapping>		m_buttonsMapping;	///<Tablica mapowania przycisk�w fizycznych na wirtualne
	std::vector<InputMapping>		m_axisMapping;		///<Tablica mapowania osi fizycznych na wirtualne
	std::vector<EventMapping>		m_requestedEvents;	///<Tablica ��danych event�w

public:
	InputAbstractionLayer();
	~InputAbstractionLayer();

	void inline set_active			( bool activate ) { active = activate; if( active ) m_activeChanged = true; }	///<Ustawia warstw� abstrakcji jako aktywn�
	void inline set_mouse_position	( short X, short Y ) { mouseX = X; mouseY = Y; }								///<Ustawia pozycj� myszy

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

