#pragma once
#include "stdafx.h"
#include "Interfaces\basic_interfaces.h"

class Engine;


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
	unsigned int add_moveable_object(DynamicObject* dyn_object);
};


