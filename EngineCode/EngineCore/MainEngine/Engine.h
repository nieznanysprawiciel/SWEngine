#pragma once
/**@file Engine.h
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.

@brief Plik zawiera deklaracjê g³ównego obiektu silnika.*/

#include "EngineCore/SW_engine.h"
#include "EngineCore/MainEngine/EngineInterface.h"
#include "Common/macros_switches.h"


class IGamePlay;
class Object;


//	window defines
#define		MAX_LOADSTRING			100

//	engines defines
#define		NUM_OF_MODULES			9


/**@defgroup EngineCore
@ingroup ModulesStructure
@brief Podstawowe funkcjonalnoœci silnika SWEnigne.*/

/**@brief Enumeracja zawieraj¹ca identyfikatory dla poszczegolnych modu³ów silnika.*/
typedef enum
{
	E_CONTROLLERS = 0,			///<Klasa ControllersEngine
	E_PHYSIC = 1,				///<Klasa PhysicEngine
	E_FABLE = 2,				///<Klasa FableEngine
	E_MOVEMENT_DYNAMIC = 3,		///<Obiekty dynamiczne w klasie MovementEngine
	E_MOVEMENT_COMPLEX = 4,		///<Obiekty z³o¿one w klasie MovementEngine
	E_DISPLAY = 5,				///<Klasa DisplayEngine
	E_COLLISION = 6,			///<Klasa CollisionEngine
	E_UI = 7,					///<Klasa UI_engine
	E_MODELS_MANAGER = 8		///<Klasa ModelsManager
} Modules;



//global variables
extern TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
extern TCHAR szWindowClass[MAX_LOADSTRING];				// the main window class name


/**@brief Klasa Engine jest nadrzêdnym obiektem zarz¹dzaj¹cym wszystkimi modu³ami silnika.
W aplikacji powinien istnieæ tylko jeden obiekt podanego typu. O tym, jak siê inicjuje dzia³anie silnika,
mo¿na przeczytaæ w temacie Inicjowanie silnika.

W tej klasie powinny byæ umieszczane wszystkie funkcje, które nie powinny byæ dostêpne
dla u¿ytkownika silnika. Interfejsem dla u¿ytkownika jest klasa @ref EngineInterface i tam nale¿y umieszczaæ
pozosta³e funkcje. Co do zmiennych klasy, wszystkie powinny siê znaleŸæ w klasie EnigneInterface, aby wszystkie
funkcje mog³y mieæ do nich dostêp.
*/
class Engine : public EngineInterface
{
private:		//zmienne, które nie maj¹ prawa zostaæ u¿yte przez EngineInterface


private:
	/**@name Funkcje do obs³ugi okna aplikacji*/
	///@{
	ATOM EngineRegisterClass();					///<Rejestruje klasê okna aplikacji.
	BOOL InitInstance( int nCmdShow );			///<Inicjuje okno aplikacji.
public:
	void ShowAppWindow( int showFlags );		///<Pokazuje okno aplikacji na ekranie.
	void HideAppWindow();						///<Chowa okno aplikacji.
	void EndAplication();
	int MainLoop();
	///@}
public:
	///@name Funkcje zwi¹zane z renderowaniem
	///@{
	void		RenderFrame					();
	void		UpdateScene					( float& lag, float timeInterval );
	void		RenderScene					( float lag, float timeInterval );
	void*		GetRenderTargetHandle		( uint16 width, uint16 height );
	///@}
#ifndef __UNUSED
	//render thread joining
	bool join_render_thread;
#endif

	Engine( const Engine& ) = delete;		///<Konstruktor kopiuj¹cy usuniêty.
public:
	Engine( HINSTANCE instance );
	~Engine();

	int InitEngine				( int width, int height, bool full_screen, int nCmdShow );
private:
	///@name Funkcje inicjuj¹ce modu³y silnika
	///@{
	BOOL InitWindow				( int width, int height, BOOL full_screen, int nCmdShow );
	bool InitGraphicAPI			( int width, int height, bool full_screen );
	bool InitInputModule		();
	bool InitSoundModule		();

	bool InitDefaultAssets		();
	bool InitDisplayer			();
	///@}
public:
#ifndef __UNUSED
	//threads
	void render_thread();
	unsigned int get_efficient_threads_count();
	void wait_for_threads();
#endif


	//Engine functions
#ifndef __UNUSED
	void time_controller(float& time_interval);
#endif

	///@name Obs³uga eventów
	///@{
	void							send_event				( Event* new_event );

	inline std::queue<Event*>*		get_events_queue		()									{ return events_queue; }
	inline void						set_events_queue		( std::queue<Event*>* queue )		{ events_queue = queue; }
	///@}

	///@name Interfejs do ustawiania GamePlaya
	///@{
	void							set_entry_point			( IGamePlay* game_play );
	void							set_entry_point			( const std::wstring dll_name );
	///@}

#ifdef __TEST
	void test();
	std::vector<Object*>	object_list;
#endif

	///@name Parametry okna
	///@{
	inline HINSTANCE				get_instance_handler()								{ return instance_handler; }
	inline HWND						get_window_handler()								{ return window_handler; }

	inline int						get_window_widht()									{ return window_width; }
	inline int						get_window_height()									{ return window_height; }
	///@}
};

/**Celem wprowadzenia wektora index_predictor jest optymalizacja dostêpu do danych 
 *obiektów w modu³ach silnika. Ka¿dy modu³ zawiera tablicê obiektów, które ma obs³ugiwaæ.
 *Obiekty mog¹ byæ do tych tablic dodawane i usuwane.
 *Z tego powodu nie jesteœmy w stanie ca³y czas przechowywaæ aktualnego indeksu obiektów w kazdym module,
 *ale charakterystyka operacji, które wykonujemy, pozwala na przewidzenie, na jakiej pozycji mniej wiêcej znajduje siê
 *dany obiekt.
 *Zauwa¿my, ¿e je¿eli umieœcimy jakiœ obiekt na pozycji n-tej, to ten indeks nigdy nie bêdzie wiêkszy, bo nigdy nie wstawiamy
 *elementów w œrodek tablicy, tylko zawsze na koniec. Kasuj¹c elementy wczesniejsze, przesuwamy obiekt w kierunku numerów mniejszych od n.
 *Z tego wynika, ¿e najbardziej efektywne bêdzie przeszukiwanie od pozycji n-tej w dó³.
 *
 *Regu³a ta nie dzia³a w przypadku klasy DisplayEngine. Tablica jest tam u³o¿ona w celu optymalizacji wyœwietlania, co oznacza,
 *¿e niektóre obiekty trzeba bêdzie wstawiaæ w œrodek tablicy.
 *W tej sytuacji wyjœciem jest przeszukiwanie dwukierunkowe od indeksu z numerem n.
 *
 *Ponadto, aby trzymaæ tablicê index_predictor jak najbardziej aktualn¹, ka¿da funkcja, która odwo³uje siê do danego elementu
 *powinna aktualizowac jego indeks.
*/
struct IndexPrediction
{
	Object*				object_ptr;
	unsigned int		index[NUM_OF_MODULES];
};
