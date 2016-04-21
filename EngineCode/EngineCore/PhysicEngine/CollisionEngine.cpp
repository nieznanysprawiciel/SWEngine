#include "EngineCore/stdafx.h"
#include "CollisionEngine.h"


#include "Common/MemoryLeaks.h"


CollisionEngine::CollisionEngine(Engine* engine)
	: engine(engine)
{
	m_octree = new CollisionOctree();
}


CollisionEngine::~CollisionEngine()
{
	delete m_octree;
}



void CollisionEngine::ProceedCollisions(float time_interval)
{

}