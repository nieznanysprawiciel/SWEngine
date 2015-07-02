#pragma once

class Engine;

class PhysicEngine
{
private:
	Engine* engine;
public:
	PhysicEngine(Engine* engine);
	~PhysicEngine();

	void proceed_physic(float time_interval);
};

