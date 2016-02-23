#pragma once
/**@file CollisionEngine.h
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.

@brief Modu³ CollisionEngine.*/

class Engine;

/**@brief Modu³ obs³uguj¹cy kolizje.
@ingroup EngineCore*/
class CollisionEngine
{
private:
	Engine* engine;
public:
	CollisionEngine(Engine* engine);
	~CollisionEngine();

	void proceed_collisions(float time_interval);
};

