/**
@file PhysicEngine.cpp
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/

#include "EngineCore/stdafx.h"
#include "PhysicEngine.h"


#include "swCommonLib/Common/MemoryLeaks.h"


namespace sw
{

// ================================ //
//
PhysicEngine::PhysicEngine( Engine* engine )
	: engine( engine )
{}


// ================================ //
//
PhysicEngine::~PhysicEngine()
{}

/*G��wna funkcja wykonywana w ka�dym obiegu p�tli*/
void			PhysicEngine::ProceedPhysic			( TimeType time, TimeDiff elapsed )
{

}

// ================================ //
//
void			PhysicEngine::RemoveActor			( ActorBase * actor )
{
	// Zaimplementowa�
}

// ================================ //
//
void			PhysicEngine::RemoveAllActors		()
{
	// Zaimplementowa�
}


}	// sw
