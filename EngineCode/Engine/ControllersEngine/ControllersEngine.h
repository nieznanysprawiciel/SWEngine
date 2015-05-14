#pragma once
#include "stdafx.h"
#include "Interfaces\basic_interfaces.h"

class Engine;

class ControllersEngine
{
private:
	Engine* engine;

	std::vector<Dynamic_object*>	pre_controlled_objects;
	std::vector<Dynamic_object*>	post_controlled_objects;
public:
	ControllersEngine(Engine* parent);
	~ControllersEngine();

	void proceed_controllers_pre(float time_interval);
	void proceed_controllers_post( float time_interval );

	int add_pre_controlled( Dynamic_object* object );
	int add_post_controlled( Dynamic_object* object );
};

