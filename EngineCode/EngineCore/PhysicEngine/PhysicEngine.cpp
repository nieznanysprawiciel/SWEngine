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

/*G��wna funkcja wykonywana w ka�dym obiegu p�tli*/
void PhysicEngine::proceed_physic(float time_interval)
{

}