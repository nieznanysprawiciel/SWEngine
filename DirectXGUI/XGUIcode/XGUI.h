/**
@file XGUI.h
@brief Ten plik nale�y za��czy� w projekcie u�ywaj�cym biblioteki DirectXGUI.
Zawiera podstawowe deklaracje.*/

#include <D3DX11.h>
#include <DirectXMath.h>


namespace XGUI
{
	class Control;
	class Root;


	/**@brief Struktura opisuj�ca czworok�t.*/
	struct Rect
	{
		DirectX::XMFLOAT2 top_left;		///<Wsp�rz�dne lewego g�rnego rogu.
		DirectX::XMFLOAT2 bottom_right;	///<Wsp�rz�dne prawego dolnego rogu.
	};

	/**@brief Enumeracja opisuj�ca mo�liwe wyr�wnanie kontrolek wzgl�dem rodzica.
	
	Zmienne enumeracj� mo�na sk�ada� za pomoca operatora sumy bitowej.
	U�ytkownik jest odpowiedzialny za sk�adanie zmiennej z sensem.*/
	enum ALIGNMENT
	{
		LEFT			= 1,
		RIGHT			= 2,
		TOP				= 4,
		TOP_LEFT		= LEFT | TOP,
		TOP_RIGHT		= RIGHT | TOP,
		BOTTOM			= 8,
		BOTTOM_LEFT		= BOTTOM | LEFT,
		BOTTOM_RIGHT	= BOTTOM | RIGHT
	};


	/**@brief Klasa bazowa dla wszystkich kontrolek.*/
	class Control
	{
	private:
		static Root*			root;		///<Korze� drzewa kontrolek b�d�cy jednocze�nie zarz�dc� ca�ego GUI

	protected:
		Control*				parent;		///<Rodzic kontrolki.

		bool					visible;	///<Widoczno�� kontrolki
		bool					mouse_on;	///<Przy ostatnim sprawdzaniu myszka by�a w obszarze kontrolki.

		DirectX::XMFLOAT2		relative_position;	///<Pozycja wzgl�dem rodzica (we wsp�rz�dnych ekranu [-1;1])
		DirectX::XMFLOAT2		dimension;	///<Rozmiar kontrolki (we wsp�rz�dnych ekranu [-1;1])
		ALIGNMENT				align;		///<Identyfikuje wzgl�dem czego s� podawane wsp�rz�dne position
	public:
		inline bool is_mouse_on() { return mouse_on; }	///<Funkcja zwraca warto�� zmiennej mouse_on
		inline bool is_visible() { return visible; }	///<Funkcja zwraca warto�� pola visible

		/**@brief Funkcja sprawdza czy mysz znajduje si� wewn�trz obszaru danej kontrolki.

		Domy�lnie zaimplementowane jest sprawdzanie czy kursor znajduje si� w odpowiednim
		prostok�cie. Je�eli kszta�t kontrolki jest inny, mo�na t� funkcj� przeimplementowa�
		dla tego kszta�tu.

		Po�o�enie kontrolki jest obliczane wzgledem podanego prostok�ta w parametrze clipping_rect.
		Zmienna align m�wi o tym, wzgl�dem kt�rej kraw�dzi zdefiniowane jest po�o�enie
		w wektorze relative_position.

		@attention
		Obiekt nie sprawdza czy mysz znalaz�a si� w jego obszarze, je�eli nie jest widoczny.

		@note
		Funkcja nie powinna zmienia� stanu obiektu. Po sprawdzeniu czy mysz jest w obszarze
		kontrolki nast�puj� najcz�ciej wywo�anie funkcji on_mouse_reaction. Tam nale�y implementowa�
		reakcj� na mysz.

		@note
		Nie zawsze musimy zwr�ci� prawdziw� informacj�. Je�eli chcemy zaimplementowa� zmian�
		rozmiar�w kontrolki, to mysz cz�sto b�dzie wyje�d�a� poza jej obszar. Wtedy musimy
		zwr�ci� true, mimo �e mysz nie znajduje si� w naszym obszarze.

		@param point Punkt w pikselach, w kt�rych znajduje si� mysz.
		@param clipping_rect Obszar rodzica wzgl�dem kt�rego jest liczone po�o�enie kontrolki.
		@return Zwraca true, je�eli mysz znajduje si� w obszarze zajmowanym prze kontrolk�.
		*/
		virtual bool test_mouse( const DirectX::XMFLOAT2& point, const Rect& clipping_rect);

		/**@brief Funkcja implementuje reakcj� kontrolki na to, �e mysz znalaz�a si� w jej obszarze.

		Najpierw kontrolka jest sprawdzana funkcj� test_mouse. Je�eli zwr�ci true, to powinna zosta�
		wywo�ana funkcja on_mouse_reaction, w kt�rej znajduje si� reakcja na najechanie mysz�.

		@param point Punkt w pikselach, w kt�rych znajduje si� mysz.
		@param clipping_rect Obszar rodzica wzgl�dem kt�rego jest liczone po�o�enie kontrolki.
		@param buttons Tablica przycisk�w. @todo: Trzeba wymy�le� format tej tablicy
		*/
		virtual void on_mouse_reaction( const DirectX::XMFLOAT2& point, const Rect& clipping_rect, const char* buttons );

		/**@brief Funkcja zostaje wywo�ana, kiedy mysz najedzie na kontrolk�. Wywo�ywana tylko
		raz za pierwszym razem, kiedy zostanie wykryta mysz nad kontrolk�.

		Funkcja pozwala dowolnej kontrolce na zaimplementowanie jakiej� typowej dla siebie funkcjonalno�ci,
		wywo�ywanej przez najechanie mysz�, bez przedefiniowywania funkcji on_mouse_reaction.
		Tak� funkcjonalno�ci� mo�e by� np. zmiana wygl�du kontrolki po najechaniu i po zjechaniu myszy
		z jej obszaru ( @ref mouse_out ).

		@attention
		Do renderingu przeznaczona jest tylko i wy��cznie funkcja @ref draw_clipped. W tej funkcji mo�na
		co najwy�ej ustawi� odpowiedni stan obiektu, �eby p�niej zosta� on poprawnie wyrenderowany.

		@note
		Informacja o zaj�ciu zdarzenia zostaje r�wnie� przekazana na zewn�trz GUI w postaci wywo�ania
		odpowiedniej funkcji obs�ugi lub wys�ania eventu do kolejki komunikat�w. Nie nale�y samodzielnie
		implementowa� takiego zachowania, poniewa� jest ono zaimplementowane w funkcji on_mouse_reaction.

		@param point Punkt w pikselach, w kt�rych znajduje si� mysz.
		@param clipping_rect Obszar rodzica wzgl�dem kt�rego jest liczone po�o�enie kontrolki.
		@param buttons Tablica przycisk�w. @todo: Trzeba wymy�le� format tej tablicy
		*/
		virtual void mouse_on( const DirectX::XMFLOAT2& point, const Rect& clipping_rect, const char* buttons ) = 0;
		
		/**@brief Funkcja wywo�ywana w momencie przekazania kontrolce focusa.

		Funkcja pozwala dowolnej kontrolce na zaimplementowanie jakiej� typowej dla siebie funkcjonalno�ci
		w reakcji na otrzymanie focusa.

		@note
		Informacja o zaj�ciu zdarzenia zostaje r�wnie� przekazana na zewn�trz GUI w postaci wywo�ania
		odpowiedniej funkcji obs�ugi lub wys�ania eventu do kolejki komunikat�w. Nie nale�y samodzielnie
		implementowa� takiego zachowania, poniewa� jest ono zaimplementowane w funkcji on_mouse_reaction.

		@param prev_focus Zwraca wska�nik na kontrolk�, kt�ra poprzednio mia�a ustawionego focusa.
		*/
		virtual void focus_set( Control* prev_focus ) = 0;

		/**@brief Funkcja wywo�ywana w momencie, gdy kontrolka traci focusa.

		Funkcja pozwala dowolnej kontrolce na zaimplementowanie jakiej� typowej dla siebie funkcjonalno�ci
		w reakcji na utrat� focusa.

		@attention
		Funkcja nie powinna pr�bowa� przywraca� sobie focusa. Istanieje niebezpiecze�stwo zap�tlenia
		lub nieprzewidywalnego zachowania, je�eli dwie kotrolki b�d� pr�bowa�y wykona� t� operacj�.

		@note
		Informacja o zaj�ciu zdarzenia zostaje r�wnie� przekazana na zewn�trz GUI w postaci wywo�ania
		odpowiedniej funkcji obs�ugi lub wys�ania eventu do kolejki komunikat�w. Nie nale�y samodzielnie
		implementowa� takiego zachowania, poniewa� jest ono zaimplementowane w funkcji on_mouse_reaction.

		@param next_focus Wska�nik na kontrolk�, kt�ra otrzyma�a focusa.
		*/
		virtual void focus_lost( Control* next_focus ) = 0;

		/**@brief Funkcja wywo�ywana w momencie, gdy kontrolka zostanie klikni�ta lewym przyciskiem myszy.

		Funkcja pozwala dowolnej kontrolce na zaimplementowanie jakiej� typowej dla siebie funkcjonalno�ci
		w reakcji na klikni�cie.

		@note
		Informacja o zaj�ciu zdarzenia zostaje r�wnie� przekazana na zewn�trz GUI w postaci wywo�ania
		odpowiedniej funkcji obs�ugi lub wys�ania eventu do kolejki komunikat�w. Nie nale�y samodzielnie
		implementowa� takiego zachowania, poniewa� jest ono zaimplementowane w funkcji on_mouse_reaction.

		@param point Punkt w pikselach, w kt�rych znajduje si� mysz.
		@param clipping_rect Obszar rodzica wzgl�dem kt�rego jest liczone po�o�enie kontrolki.
		@param buttons Tablica przycisk�w. @todo: Trzeba wymy�le� format tej tablicy
		*/
		virtual void on_click( const DirectX::XMFLOAT2& point, const Rect& clipping_rect, const char* buttons );

		/**@brief Lewy przycisk myszy zosta� puszczony.

		Funkcja pozwala dowolnej kontrolce na zaimplementowanie jakiej� typowej dla siebie funkcjonalno�ci
		w reakcji na puszczenie przycisku.

		@attention
		Funkcja nie zostanie wywo�ana, je�eli puszczenie przycisku nie nast�pi nad kontrolk�. W przypadku
		przeniesienia si� znad obszaru jednej kontrolki nad inn� z wci�ni�tym przyciskiem, pierwsza kontrolka
		dostanie zdarzenie click, a druga un_click.

		@note
		Informacja o zaj�ciu zdarzenia zostaje r�wnie� przekazana na zewn�trz GUI w postaci wywo�ania
		odpowiedniej funkcji obs�ugi lub wys�ania eventu do kolejki komunikat�w. Nie nale�y samodzielnie
		implementowa� takiego zachowania, poniewa� jest ono zaimplementowane w funkcji on_mouse_reaction.

		@param point Punkt w pikselach, w kt�rych znajduje si� mysz.
		@param clipping_rect Obszar rodzica wzgl�dem kt�rego jest liczone po�o�enie kontrolki.
		@param buttons Tablica przycisk�w. @todo: Trzeba wymy�le� format tej tablicy
		*/
		virtual void un_click( const DirectX::XMFLOAT2& point, const Rect& clipping_rect, const char* buttons );

		/**@brief Funkcja wywo�ywana w momencie, gdy kontrolka zostanie klikni�ta prawym przyciskiem myszy.

		Funkcja pozwala dowolnej kontrolce na zaimplementowanie jakiej� typowej dla siebie funkcjonalno�ci
		w reakcji na klikni�cie.

		@note
		Informacja o zaj�ciu zdarzenia zostaje r�wnie� przekazana na zewn�trz GUI w postaci wywo�ania
		odpowiedniej funkcji obs�ugi lub wys�ania eventu do kolejki komunikat�w. Nie nale�y samodzielnie
		implementowa� takiego zachowania, poniewa� jest ono zaimplementowane w funkcji on_mouse_reaction.

		@param point Punkt w pikselach, w kt�rych znajduje si� mysz.
		@param clipping_rect Obszar rodzica wzgl�dem kt�rego jest liczone po�o�enie kontrolki.
		@param buttons Tablica przycisk�w. @todo: Trzeba wymy�le� format tej tablicy
		*/
		virtual void right_click( const DirectX::XMFLOAT2& point, const Rect& clipping_rect, const char* buttons );

		/**@brief Prawy przycisk myszy zosta� puszczony.

		Funkcja pozwala dowolnej kontrolce na zaimplementowanie jakiej� typowej dla siebie funkcjonalno�ci
		w reakcji na puszczenie przycisku.

		@attention
		Funkcja nie zostanie wywo�ana, je�eli puszczenie przycisku nie nast�pi nad kontrolk�. W przypadku 
		przeniesienia si� znad obszaru jednej kontrolki nad inn� z wci�ni�tym przyciskiem, pierwsza kontrolka
		dostanie zdarzenie click, a druga un_click.

		@note
		Informacja o zaj�ciu zdarzenia zostaje r�wnie� przekazana na zewn�trz GUI w postaci wywo�ania
		odpowiedniej funkcji obs�ugi lub wys�ania eventu do kolejki komunikat�w. Nie nale�y samodzielnie
		implementowa� takiego zachowania, poniewa� jest ono zaimplementowane w funkcji on_mouse_reaction.

		@param point Punkt w pikselach, w kt�rych znajduje si� mysz.
		@param clipping_rect Obszar rodzica wzgl�dem kt�rego jest liczone po�o�enie kontrolki.
		@param buttons Tablica przycisk�w. @todo: Trzeba wymy�le� format tej tablicy
		*/
		virtual void right_un_click( const DirectX::XMFLOAT2& point, const Rect& clipping_rect, const char* buttons );

		/**@brief Funkcja u�ywana do zakomunikowania dzieciom jakiej� kontrolki, �e mysz opu�ci�a
		obszar rodzica.

		Funkcja jest wywo�ywana w trzech sytuacjach:
		- Rodzic kontrolki przy poprzednim wywo�aniu test_mouse mia� nad sob� kursor myszy, a w kolejnym
		go utraci�. Wtedy musi on wywo�a� mouse_out dla wszystkich swoich dzieci, kt�re tak�e mia�y
		nad sob� kursor, sprawdzaj�c warto�� zmiennej mouse_on.
		- Rodzic kontrolki otrzyma� komunikat mouse_out, wi�c przekazuje je wszystkim potomkom, kt�rzy
		maj� ustawion� zmienn� mouse_on na true.
		- Rodzic odpytuj�c swoje dzieci znalaz� ju� kontrolk� w kt�rej obszarze znajduje si� mysz.
		Musi teraz poinformowa� rodze�stwo tamtej kontrolki, �e utraci�y mysz je�eli j� mia�y.

		Mysz mo�e si� znajdowa� jednocze�nie w obszarze tylko jednej kontrolki spo�r�d dzieci.
		Jednak poniewa� kotrolki mog� si� przys�ania�, to funkcja test_mouse mo�e wielokrotnie
		zwr�ci� warto�� true. Z tego wzgl�du bierze si� pod uwag� zawsze pierwsz� kontrolk� na li�cie,
		kt�ra zwr�ci te warto��.

		@param point Wsp�rz�dne myszy.
		*/
		virtual void mouse_out( const DirectX::XMFLOAT2& point );


		/**@brief Rysuje kontrolk� wzgl�dem czworok�ta clipping_rect.

		Funkcja nie ma prawa wyj�� poza podany w parametrze czworok�t. Je�eli wyjdzie
		to mo�e narysowa� si� na miejscu zajmowanym przez jak�� inn� kontrolk�.

		Kontrolka ma si� na rysowa� w po�o�eniu okre�lanym przez zmienn� position wzgl�dem tej kraw�dzi,
		na kt�ra wskazuje zmienna align. Wszystkim obiektom potomnym nale�y poda� sw�j w�asny obszar obcinania
		i wywo�a� funkcj� rekurencyjnie. Nale�y pami�ta� o w�a�ciwej kolejno�ci wywo�ania, aby kontrolki
		w�a�ciwie si� przys�ania�y. Co prawda kontrolki musz� by� wewn�trz obszaru rodzica, ale nie ma gwarancji,
		�e na siebie nie wchodz�. Je�eli w funkcji test_mouse schodzono rekurencyjnie od pocz�tku listy,
		to rysowa� wypada w odwrotnej kolejno�ci.

		@attention
		Kontrolka nie jest rysowana, je�eli zmienna visible jest ustawiona na false.

		@param clipping_rect Czworok�t, wewn�trz kt�rego ma zosta� narysowana kontrolka.
		*/
		virtual void draw_clipped( const Rect& clipping_rect );
	};


	/**@brief Korze� drzewa kontrolek i jednocze�nie klasa zarz�dzaj�ca ca�ym GUI.
	*/
	class Root	:	public Control
	{
	private:
		Control*		focus;				///<Wskazuje na kontrolk�, kt�ra posiada aktualnie focusa lub nullptr

	protected:

	public:
		void set_focus( Control* control );		///<Ustawia focus dla podanej w parametrze kontrolki.

	};

}