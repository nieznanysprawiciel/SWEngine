#include "EngineCore/stdafx.h"
#include "CollisionEngine.h"


#include "Common\memory_leaks.h"


CollisionEngine::CollisionEngine(Engine* engine)
	: engine(engine)
{
}


CollisionEngine::~CollisionEngine()
{
}



void CollisionEngine::proceed_collisions(float time_interval)
{

}