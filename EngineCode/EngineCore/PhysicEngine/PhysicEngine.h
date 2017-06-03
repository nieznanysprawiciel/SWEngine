#pragma once
/**
@file PhysicEngine.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/


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

	void		ProceedPhysic			( float timeInterval );

	void		RemoveActor				( ActorBase* actor );
	void		RemoveAllActors			();
};


}	// sw

