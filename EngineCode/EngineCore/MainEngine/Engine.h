#pragma once
/**@file Engine.h
@author nieznanysprawiciel
@copyright Plik jest cz�ci� silnika graficznego SWEngine.

@brief Plik zawiera deklaracj� g��wnego obiektu silnika.*/


#define WIN32_LEAN_AND_MEAN
#include <Windows.h>		///< @todo Delete in future

#undef min
#undef max
#undef CreateWindow

#include "EngineCore/MainEngine/MacrosSwitches.h"

#include "EngineCore/SW_engine.h"
#include "EngineCore/MainEngine/EngineInterface.h"
#include "swInputLibrary/InputCore/IInput.h"

#include <queue>

class IGamePlay;
class ActorBase;
class Event;
class IInput;
struct CameraData;

//	engines defines
#define		NUM_OF_MODULES			9


/**@defgroup EngineCore
@ingroup ModulesStructure
@brief Podstawowe funkcjonalno�ci silnika SWEngine.*/



/**@brief Klasa Engine jest nadrz�dnym obiektem zarz�dzaj�cym wszystkimi modu�ami silnika.
@ingroup EngineCore
@copybrief

W tej klasie powinny by� umieszczane wszystkie funkcje, kt�re nie powinny by� dost�pne
dla modu��w logiki gry oraz aktor�w. Stanowi� one interfejs dla funkcji main oraz
ewentualnie dla edytora.
@todo Zastanowi� si� czy edytor nie powinien si� komunikowa� z silnikiem za pomoc� zwyk�ego API.

W aplikacji powinien istnie� tylko jeden obiekt podanego typu. O tym, jak si� inicjuje dzia�anie silnika,
mo�na przeczyta� w temacie @ref EngineInit.

Interfejsem dla u�ytkownika jest klasa @ref EngineInterface i tam nale�y umieszcza�
wszystkie funkcje, kt�re maj� by� dost�pne dla logiki gry. Nie nale�y w tej klasie umieszcza� zmiennych,
kt�re mog� by� przydatne klasie EngineInterface.
*/
class Engine : public EngineInterface
{
private:		//zmienne, kt�re nie maj� prawa zosta� u�yte przez EngineInterface


private:
	/**@name Funkcje do obs�ugi okna aplikacji*/
	///@{
	ATOM		EngineRegisterClass		();						///<Rejestruje klas� okna aplikacji.
	BOOL		InitInstance			( int nCmdShow );		///<Inicjuje okno aplikacji.
public:
	void		ShowAppWindow			( int showFlags );		///<Pokazuje okno aplikacji na ekranie.
	void		HideAppWindow			();						///<Chowa okno aplikacji.
	void		EndAplication			();
	int			MainLoop				();
	///@}
public:
	///@name Funkcje zwi�zane z renderowaniem
	///@{
	void		RenderFrame					();
	void		UpdateScene					( float& lag, float timeInterval );
	void		SingleThreadedUpdatePhase	( float& lag, float timeInterval );
	void		RenderScene					( float lag, float timeInterval );
	///@}

	///@name Funkcje dla EditorPlugin
	///@brief Funkcje mog� by� niebezpieczne w u�ytku, jak si� nie wie co si� robi. U�ywa� ostro�nie!
	///@{
	void*		GetRenderTargetHandle		( uint16 width, uint16 height );
	CameraData&	GetMainCamera				();
	///@}


#ifndef __UNUSED
	//render thread joining
	bool join_render_thread;
#endif

	Engine( const Engine& ) = delete;		///<Konstruktor kopiuj�cy usuni�ty.
public:
	explicit Engine();
	explicit Engine( HINSTANCE instanceHandle );
			~Engine();

	bool	InitEngine				( int nCmdShow );
	int		InitEngine				( int width, int height, bool fullscreen, int nCmdShow );
private:
	///@name Funkcje inicjuj�ce modu�y silnika
	///@{
	void	InternalInit			( HINSTANCE instanceHandle );

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


	///@name Obs�uga event�w
	///@{
	void							SendEvent				( Event* newEvent );
	///@}

	///@name Interfejs do ustawiania GamePlaya
	///@{
	void							SetEntryPoint			( IGamePlay* game_play );
	void							SetEntryPoint			( const std::wstring dll_name );
	///@}

#ifdef __TEST
	void		test				();
	void		testMaterial		( Model3DFromFile* model );
	void		SetSkydomeAndCamera	();
	void		SetSkydome			();
	void		SetCamera			();
	void		CreateDefaultScene	();
#endif

	///@name Parametry okna
	///@{
	AppInstanceHandle		GetInstanceHandler();
	WindowHandle			GetWindowHandler();

	int						GetWindowWidth();
	int						GetWindowHeight();
	///@}

	///@name Input
	///@{
	void					EnableInput			( bool val );
	IInput*					ChangeInputModule	( IInput* newModule );
	///@}

	///@name For unit tests
	///@{
	AssetsManager*			GetAssetsManager	();
	///@}
};


