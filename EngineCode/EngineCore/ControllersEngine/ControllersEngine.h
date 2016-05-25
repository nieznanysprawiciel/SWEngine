#pragma once
#include "EngineCore/stdafx.h"
#include "EngineCore/Actors/ActorObjects.h"
#include "BaseClasses/IController.h"

class Engine;

/**@brief Modu� do zarz�dzania kontrolerami obiekt�w.
@ingroup EngineCore*/
class ControllersEngine
{
private:
	Engine* engine;

	std::vector<DynamicActor*>		m_preControlledObjects;
	std::vector<DynamicActor*>		m_postControlledObjects;

public:
	ControllersEngine( Engine* parent );
	~ControllersEngine();

	void		ProceedControllersPre			( float timeInterval );
	void		ProceedControllersPost			( float timeInterval );

	bool		AddPreControlled				( DynamicActor* object );
	bool		AddPostControlled				( DynamicActor* object );
};

