#include "EngineCore/stdafx.h"
#include "PhysicEngine.h"


#include "Common\memory_leaks.h"


PhysicEngine::PhysicEngine(Engine* engine)
	: engine(engine)
{
}


PhysicEngine::~PhysicEngine()
{
}

/*G³ówna funkcja wykonywana w ka¿dym obiegu pêtli*/
void PhysicEngine::proceed_physic(float time_interval)
{

}