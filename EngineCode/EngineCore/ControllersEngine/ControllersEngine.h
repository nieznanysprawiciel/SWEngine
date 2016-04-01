#pragma once
#include "EngineCore/stdafx.h"
#include "EngineCore/Actors/ActorObjects.h"

class Engine;

/**@brief Modu³ do zarz¹dzania kontrolerami obiektów.
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

	void		ProceedControllersPre			( float time_interval );
	void		ProceedControllersPost			( float time_interval );

	bool		AddPreControlled				( DynamicActor* object );
	bool		AddPostControlled				( DynamicActor* object );
};

