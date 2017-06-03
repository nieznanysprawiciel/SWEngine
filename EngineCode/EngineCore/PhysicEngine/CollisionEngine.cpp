/**
@file CollisionEngine.cpp
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/


#include "EngineCore/stdafx.h"
#include "CollisionEngine.h"

#include "EngineCore/Actors/ActorObjects.h"

#include "swCommonLib/Common/MemoryLeaks.h"


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

void CollisionEngine::RemoveActor( ActorBase* actor )
{
	// Zaimplementowaæ.
}

void CollisionEngine::RemoveAllActors()
{
	// Zaimplementowaæ.
}
