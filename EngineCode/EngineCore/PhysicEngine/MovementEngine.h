#pragma once
/**
@file MovementEngine.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/


#include "EngineCore/Actors/ActorObjects.h"

#include "EngineCore/MainEngine/Types.h"




namespace sw
{


class Engine;


/**@brief Modu� do obs�ugi obiekt�w dynamicznych.
@ingroup EngineCore*/
class MovementEngine
{
private:
	Engine* engine;

	std::vector< DynamicActor* >			m_dynamicObjects;
	std::vector< ComplexActor *>			m_complexObjects;
public:
	MovementEngine( Engine* engine );
	~MovementEngine();

	void		ProceedMovement			( TimeType time, TimeDiff elapsed );
	Size		AddMoveableObject		( DynamicActor* dynObject );

	void		RemoveActor				( ActorBase* actor );
	void		RemoveAllActors			();
};


}	// sw
