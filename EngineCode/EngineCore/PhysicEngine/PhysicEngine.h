#pragma once

class Engine;


/**@brief Modu� fizyki.
@ingroup EngineCore*/
class PhysicEngine
{
private:
	Engine* engine;
public:
	PhysicEngine(Engine* engine);
	~PhysicEngine();

	void proceed_physic(float time_interval);
};

