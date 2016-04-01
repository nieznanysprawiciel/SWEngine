#include "EngineCore/stdafx.h"
#include "MovementEngine.h"


#include "Common/MemoryLeaks.h"


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
		DynamicActor* object = dynamic_objects[i];
		object->Move( time_interval );
		object->swap();		// Zamieniamy bufory wewn¹trz obiektu
	}
}

Size MovementEngine::add_moveable_object(DynamicActor* dyn_object)
{
	dynamic_objects.push_back(dyn_object);
	return dynamic_objects.size() - 1;		//zwracamy indeks elementu dodanego
}