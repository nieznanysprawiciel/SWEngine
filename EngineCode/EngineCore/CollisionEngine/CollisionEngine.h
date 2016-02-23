#pragma once
/**@file CollisionEngine.h
@author nieznanysprawiciel
@copyright Plik jest cz�ci� silnika graficznego SWEngine.

@brief Modu� CollisionEngine.*/

class Engine;

/**@brief Modu� obs�uguj�cy kolizje.
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

