#pragma once
/**@file Engine.h
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.

@brief Plik zawiera deklaracjê g³ównego obiektu silnika.*/


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
@brief Podstawowe funkcjonalnoœci silnika SWEngine.*/



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
	void		SingleThreadedUpdatePhase	( float& lag, float timeInterval );
	void		RenderScene					( float lag, float timeInterval );
	///@}

	///@name Funkcje dla EditorPlugin
	///@brief Funkcje mog¹ byæ niebezpieczne w u¿ytku, jak siê nie wie co siê robi. U¿ywaæ ostro¿nie!
	///@{
	void*		GetRenderTargetHandle		( uint16 width, uint16 height );
	CameraData&	GetMainCamera				();
	///@}


#ifndef __UNUSED
	//render thread joining
	bool join_render_thread;
#endif

	Engine( const Engine& ) = delete;		///<Konstruktor kopiuj¹cy usuniêty.
public:
	explicit Engine();
	explicit Engine( HINSTANCE instanceHandle );
			~Engine();

	bool	InitEngine				( int nCmdShow );
	int		InitEngine				( int width, int height, bool fullscreen, int nCmdShow );
private:
	///@name Funkcje inicjuj¹ce modu³y silnika
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


	///@name Obs³uga eventów
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


