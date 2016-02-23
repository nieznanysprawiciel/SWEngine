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

	std::vector<DynamicObject*>			dynamic_objects;
	std::vector<ComplexObject*>		complex_objects;
public:
	MovementEngine(Engine* engine);
	~MovementEngine();

	void proceed_movement(float time_interval);
	Size add_moveable_object(DynamicObject* dyn_object);
};


