#pragma once
#include "EngineCore/stdafx.h"
#include "EngineCore/Actors/ActorObjects.h"

class Engine;


/**@brief Modu³ do obs³ugi obiektów dynamicznych.
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

	void		ProceedMovement			( float timeInterval );
	Size		AddMoveableObject		( DynamicActor* dynObject );

	void		RemoveActor				( ActorBase* actor );
	void		RemoveAllActors			();
};


