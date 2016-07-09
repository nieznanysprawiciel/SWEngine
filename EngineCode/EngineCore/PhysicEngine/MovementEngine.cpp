#include "EngineCore/stdafx.h"
#include "MovementEngine.h"

#include "EngineCore/EngineHelpers/ActorsCommonFunctions.h"

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

/**@brief Usuwa aktora z modu³u.*/
void MovementEngine::RemoveActor( ActorBase* actor )
{
	ActorsCommonFunctions::RemoveActor( m_dynamicObjects, static_cast< DynamicActor* >( actor ) );
	ActorsCommonFunctions::RemoveActor( m_complexObjects, static_cast< ComplexActor* >( actor ) );
}

/**@brief */
void MovementEngine::RemoveAllActors()
{
	m_dynamicObjects.clear();
	m_complexObjects.clear();
}
