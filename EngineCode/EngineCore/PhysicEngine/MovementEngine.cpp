#include "stdafx.h"
#include "MovementEngine.h"


#include "Common\memory_leaks.h"


MovementEngine::MovementEngine(Engine* engine)
	: engine(engine)
{
}


MovementEngine::~MovementEngine()
{
}


void MovementEngine::proceed_movement(float time_interval)
{
	for ( unsigned int i = 0; i < dynamic_objects.size(); ++i )
	{
		DynamicObject* object = dynamic_objects[i];
		object->move( time_interval );
		object->swap();		// Zamieniamy bufory wewn¹trz obiektu
	}
}

unsigned int MovementEngine::add_moveable_object(DynamicObject* dyn_object)
{
	dynamic_objects.push_back(dyn_object);
	return dynamic_objects.size() - 1;		//zwracamy indeks elementu dodanego
}