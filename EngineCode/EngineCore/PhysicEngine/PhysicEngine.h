#pragma once
/**
@file PhysicEngine.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/

#include "EngineCore/MainEngine/Types.h"

class Engine;
class ActorBase;


namespace sw
{


/**@brief Modu³ fizyki.
@ingroup EngineCore*/
class PhysicEngine
{
private:
	Engine* engine;
public:
	PhysicEngine( Engine* engine );
	~PhysicEngine();

	void		ProceedPhysic			( TimeType time, TimeDiff elapsed );

	void		RemoveActor				( ActorBase* actor );
	void		RemoveAllActors			();
};


}	// sw

