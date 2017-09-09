#pragma once
/**
@file Engine.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Engine.
*/



#include "EngineCore/MainEngine/MacrosSwitches.h"


#include "EngineCore/MainEngine/EngineInterface.h"
#include "swInputLibrary/InputCore/IInput.h"
#include "swGUI/Core/System/GUISystem.h"

#include <queue>



namespace sw
{


class IGamePlay;
class ActorBase;
class Event;
class IInput;
struct CameraData;

//	engines defines
#define		NUM_OF_MODULES			9


/**@defgroup EngineCore
@ingroup ModulesStructure
@brief Basic SWEngine functionalities.*/



/**@brief Main Engine object.
@ingroup EngineCore
@copybrief

This class implements engine initialization of GUISystem and main loop functions.
Editor should use EditorApi class to comunicate with Engine.
Game creators shouldn't touch this class. They can use @ref EngineInterface which provides API functions.*/
class Engine : public EngineInterface, public gui::GUISystem
{
private:

	///@name GUISystem functions overrides.
	///@{

	virtual	bool			Initialize		() override;
	virtual bool			OnInitialized	() override;
	virtual void			OnClosing		() override;
	virtual void			OnIdle			() override;

	///@}

public:

	void		EndAplication			();

public:

	///@name Functions connected to rendering and main loop pipeline.
	///@{
	void		RenderFrame					();
	void		UpdateScene					( float& lag, float timeInterval );
	void		SingleThreadedUpdatePhase	( float& lag, float timeInterval );
	void		RenderScene					( float lag, float timeInterval );
	///@}

public:

	Engine( const Engine& ) = delete;		///<Konstruktor kopiuj¹cy usuniêty.

public:

	explicit				Engine	();
	explicit				Engine	( int argc, char** argv, sw::gui::INativeGUI* gui );
	explicit				Engine	( AppInstanceHandle instanceHandle );
							~Engine	();

private:
	///@name Engine modules initialization functions.
	///@{
	void	InternalInit			();

	bool	InitEngineGraphicAPI	();
	bool	InitEngineInputModule	();
	bool	InitSoundModule			();

	bool	InitDefaultAssets		();
	bool	InitDisplayer			();
	bool	InitDefaultActorsClasses();
	bool	InitGUIConfiguration	();
	///@}
public:

	///@name Obs³uga eventów
	///@{
	void							SendEvent				( Event* newEvent );
	///@}

	///@name Interfejs do ustawiania GamePlaya
	///@{
	void							SetEntryPoint			( IGamePlay* game_play );
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

	///@name For unit tests
	///@{
	AssetsManager*			GetAssetsManager	();
	///@}
};


}	// sw
