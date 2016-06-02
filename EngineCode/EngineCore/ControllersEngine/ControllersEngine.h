#pragma once
#include "EngineCore/stdafx.h"
#include "EngineCore/Actors/ActorObjects.h"
#include "BaseClasses/IController.h"

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

	void		ProceedControllersPre			( float timeInterval );
	void		ProceedControllersPost			( float timeInterval );
	
	void		SingleThreadedUpdatePhase		( float timeInterval );

	bool		AddPreControlled				( DynamicActor* object );
	bool		AddPostControlled				( DynamicActor* object );


	IControllersState*		GetGlobalState		();

private:
	void		UpdateGlobalState				( float timeInterval );
};

