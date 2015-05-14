#pragma once
#include "stdafx.h"
#include "Interfaces\basic_interfaces.h"

class Engine;


class MovementEngine
{
private:
	Engine* engine;

	std::vector<Dynamic_object*>		dynamic_objects;
	std::vector<Complex_object*>		complex_objects;
public:
	MovementEngine(Engine* engine);
	~MovementEngine();

	void proceed_movement(float time_interval);
	unsigned int add_moveable_object(Dynamic_object* dyn_object);
};


