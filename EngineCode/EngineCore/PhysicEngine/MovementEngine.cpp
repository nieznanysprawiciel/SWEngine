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

/**@brief Przemieszcza obiekty na podstawie ich prêdkoœci.*/
void MovementEngine::ProceedMovement(float timeInterval)
{
	for ( unsigned int i = 0; i < m_dynamicObjects.size(); ++i )
	{
		DynamicActor* object = m_dynamicObjects[i];
		object->Move( timeInterval );
		object->swap();		// Zamieniamy bufory wewn¹trz obiektu
	}
}

/**@brief */
Size MovementEngine::AddMoveableObject(DynamicActor* dyn_object)
{
	m_dynamicObjects.push_back(dyn_object);
	return m_dynamicObjects.size() - 1;		//zwracamy indeks elementu dodanego
}