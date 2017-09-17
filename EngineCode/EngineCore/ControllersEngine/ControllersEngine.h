#pragma once
/**
@file ControllerEngine.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/


#include "EngineCore/Actors/ActorObjects.h"
#include "BaseClasses/IController.h"

#include "EngineCore/MainEngine/Types.h"


namespace sw
{



class Engine;
struct IControllersState;

/**@brief Modu³ do zarz¹dzania kontrolerami obiektów.
@ingroup EngineCore*/
class ControllersEngine
{
private:
	Engine* engine;

	std::vector<DynamicActor*>		m_preControlledObjects;
	std::vector<DynamicActor*>		m_postControlledObjects;

	IControllersState*				m_globalState;

public:
	explicit	ControllersEngine				( Engine* parent );
	~ControllersEngine				();

	void		ProceedControllersPre			( TimeType time, TimeDiff elapsed );
	void		ProceedControllersPost			( TimeType time, TimeDiff elapsed );

	void		SingleThreadedUpdatePhase		( TimeType time, TimeDiff elapsed );

	bool		AddPreControlled				( DynamicActor* actor );
	bool		AddPostControlled				( DynamicActor* actor );

	void		RemovePreCtrlActor				( ActorBase* actor );
	void		RemovePostCtrlActor				( ActorBase* actor );
	void		RemoveActor						( ActorBase* actor );
	void		RemoveAllActors					();


	IControllersState*		GetGlobalState		();

private:
	void		UpdateGlobalState				( TimeType time, TimeDiff elapsed );
};

}	// sw
