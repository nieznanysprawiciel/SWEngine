#pragma once

class Engine;
class ActorBase;

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

