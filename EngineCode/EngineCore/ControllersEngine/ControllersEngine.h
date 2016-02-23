#pragma once
#include "EngineCore/stdafx.h"
#include "EngineCore/Actors/ActorObjects.h"

class Engine;

/**@brief Modu� do zarz�dzania kontrolerami obiekt�w.
@ingroup EngineCore*/
class ControllersEngine
{
private:
	Engine* engine;

	std::vector<DynamicObject*>	pre_controlled_objects;
	std::vector<DynamicObject*>	post_controlled_objects;
public:
	ControllersEngine(Engine* parent);
	~ControllersEngine();

	void proceed_controllers_pre(float time_interval);
	void proceed_controllers_post( float time_interval );

	int add_pre_controlled( DynamicObject* object );
	int add_post_controlled( DynamicObject* object );
};

