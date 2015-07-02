#pragma once

class Engine;

class CollisionEngine
{
private:
	Engine* engine;
public:
	CollisionEngine(Engine* engine);
	~CollisionEngine();

	void proceed_collisions(float time_interval);
};

