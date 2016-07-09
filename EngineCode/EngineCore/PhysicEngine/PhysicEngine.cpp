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

/*G³ówna funkcja wykonywana w ka¿dym obiegu pêtli*/
void PhysicEngine::ProceedPhysic(float time_interval)
{

}

void PhysicEngine::RemoveActor( ActorBase * actor )
{
	// Zaimplementowaæ
}

void PhysicEngine::RemoveAllActors()
{
	// Zaimplementowaæ
}
