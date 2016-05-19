#pragma once
/**@file Engine.h
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.

@brief Plik zawiera deklaracjê g³ównego obiektu silnika.*/

#include <Windows.h>		///< @todo Delete in future

#include "Common/MacrosSwitches.h"

#include "EngineCore/SW_engine.h"
#include "EngineCore/MainEngine/EngineInterface.h"

#include <queue>

class IGamePlay;
class ActorBase;
class Event;


//	window defines
#define		MAX_LOADSTRING			100

//	engines defines
#define		NUM_OF_MODULES			9


/**@defgroup EngineCore
@ingroup ModulesStructure
@brief Podstawowe funkcjonalnoœci silnika SWEngine.*/

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
@ingroup EngineCore
@copybrief

W tej klasie powinny byæ umieszczane wszystkie funkcje, które nie powinny byæ dostêpne
dla modu³ów logiki gry oraz aktorów. Stanowi¹ one interfejs dla funkcji main oraz
ewentualnie dla edytora.
@todo Zastanowiæ siê czy edytor nie powinien siê komunikowaæ z silnikiem za pomoc¹ zwyk³ego API.

W aplikacji powinien istnieæ tylko jeden obiekt podanego typu. O tym, jak siê inicjuje dzia³anie silnika,
mo¿na przeczytaæ w temacie @ref EngineInit.

Interfejsem dla u¿ytkownika jest klasa @ref EngineInterface i tam nale¿y umieszczaæ
wszystkie funkcje, które maj¹ byæ dostêpne dla logiki gry. Nie nale¿y w tej klasie umieszczaæ zmiennych,
które mog¹ byæ przydatne klasie EngineInterface.
*/
class Engine : public EngineInterface
{
private:		//zmienne, które nie maj¹ prawa zostaæ u¿yte przez EngineInterface


private:
	/**@name Funkcje do obs³ugi okna aplikacji*/
	///@{
	ATOM		EngineRegisterClass		();						///<Rejestruje klasê okna aplikacji.
	BOOL		InitInstance			( int nCmdShow );		///<Inicjuje okno aplikacji.
public:
	void		ShowAppWindow			( int showFlags );		///<Pokazuje okno aplikacji na ekranie.
	void		HideAppWindow			();						///<Chowa okno aplikacji.
	void		EndAplication			();
	int			MainLoop				();
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

	int		InitEngine				( int width, int height, bool full_screen, int nCmdShow );
private:
	///@name Funkcje inicjuj¹ce modu³y silnika
	///@{
	BOOL	InitWindow				( int width, int height, BOOL full_screen, int nCmdShow );
	bool	InitGraphicAPI			( int width, int height, bool full_screen );
	bool	InitInputModule			();
	bool	InitSoundModule			();

	bool	InitDefaultAssets		();
	bool	InitDisplayer			();
	bool	InitDefaultActorsClasses();
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
	void							SendEvent				( Event* newEvent );

	std::queue<Event*>*				GetEventsQueue			();
	void							SetEventsQueue			( std::queue<Event*>* queue );
	///@}

	///@name Interfejs do ustawiania GamePlaya
	///@{
	void							SetEntryPoint			( IGamePlay* game_play );
	void							SetEntryPoint			( const std::wstring dll_name );
	///@}

#ifdef __TEST
	void test();
	void SetSkydomeAndCamera();
#endif

	///@name Parametry okna
	///@{
	HINSTANCE				GetInstanceHandler();
	HWND					GetWindowHandler();

	int						GetWindowWidth();
	int						GetWindowHeight();
	///@}

	void					EnableInput( bool val );
};


