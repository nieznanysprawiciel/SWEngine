#include "EngineCore/stdafx.h"
#include "PhysicEngine.h"


#include "Common/MemoryLeaks.h"


PhysicEngine::PhysicEngine(Engine* engine)
	: engine(engine)
{
}


PhysicEngine::~PhysicEngine()
{
}

/*G��wna funkcja wykonywana w ka�dym obiegu p�tli*/
void PhysicEngine::ProceedPhysic(float time_interval)
{

}

void PhysicEngine::RemoveActor( ActorBase * actor )
{
	// Zaimplementowa�
}

void PhysicEngine::RemoveAllActors()
{
	// Zaimplementowa�
}
