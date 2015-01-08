#include "..\..\stdafx.h"
#include "MovementEngine.h"


MovementEngine::MovementEngine(Engine* engine)
	: engine(engine)
{
}


MovementEngine::~MovementEngine()
{
}


void MovementEngine::proceed_movement(float time_interval)
{
	for (unsigned int i = 0; i < dynamic_objects.size(); ++i)
		dynamic_objects[i]->move(time_interval);
}

unsigned int MovementEngine::add_moveable_object(Dynamic_object* dyn_object)
{
	dynamic_objects.push_back(dyn_object);
	return dynamic_objects.size() - 1;		//zwracamy indeks elementu dodanego
}