#include "EngineCore/stdafx.h"
#include "ControllersEngine.h"
#include "EngineCore/MainEngine/Engine.h"
#include "EngineCore/Actors/ActorObjects.h"


#include "Common/MemoryLeaks.h"


ControllersEngine::ControllersEngine(Engine* parent)
{
	engine = parent;

}


ControllersEngine::~ControllersEngine()
{

}



/**@brief Wywo�uje kontrolery na obs�ugiwanych obiektach.

Funkcja wywo�ywana przed MovementEngine::ProceedMovement.
@see @ref Controllers*/
void ControllersEngine::ProceedControllersPre( float timeInterval )
{
	//todo:	przerobi� na wersj� wielow�tkow�
	for ( unsigned int i = 0; i < m_preControlledObjects.size(); ++i )
		if ( m_preControlledObjects[i]->GetController() != nullptr )
			m_preControlledObjects[i]->GetController()->ControlObjectPre( m_preControlledObjects[i] );
}

/**@brief Wywo�uje kontrolery na obs�ugiwanych obiektach.

Funkcja wywo�ywana po MovementEngine::ProceedMovement.
@see @ref Controllers*/
void ControllersEngine::ProceedControllersPost( float timeInterval )
{
	//todo:	przerobi� na wersj� wielow�tkow�
	for ( unsigned int i = 0; i < m_postControlledObjects.size( ); ++i )
		if ( m_postControlledObjects[i]->GetController( ) != nullptr )
			m_postControlledObjects[i]->GetController( )->ControlObjectPost( m_postControlledObjects[i] );
}


/**@brief Dodaje podany obiekt do kontroler�w wywo�ywanych przed przesuni�ciem obiekt�w.*/
bool ControllersEngine::AddPreControlled( DynamicActor* object )
{
	if ( object == nullptr )
		return false;

	m_preControlledObjects.push_back( object );
	return true;
}

/**@brief Dodaje podany obiekt do kontroler�w wywo�ywanych po przesuni�ciu obiekt�w.*/
bool ControllersEngine::AddPostControlled( DynamicActor* object )
{
	if ( object == nullptr )
		return false;

	m_postControlledObjects.push_back( object );
	return true;
}
