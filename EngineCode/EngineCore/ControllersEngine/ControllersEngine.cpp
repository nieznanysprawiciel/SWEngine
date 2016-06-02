#include "EngineCore/stdafx.h"
#include "ControllersEngine.h"
#include "EngineCore/MainEngine/Engine.h"
#include "EngineCore/Actors/ActorObjects.h"

#include "EngineCore/ControllersEngine/IControllersState.h"

#include "Common/MemoryLeaks.h"


ControllersEngine::ControllersEngine( Engine* parent )
{
	engine = parent;
	m_globalState = new IControllersState();
	m_globalState->Engine = engine;
}


ControllersEngine::~ControllersEngine()
{
	delete m_globalState;
}



/**@brief Wywo³uje kontrolery na obs³ugiwanych obiektach.

Funkcja wywo³ywana przed MovementEngine::ProceedMovement.
@see @ref Controllers*/
void ControllersEngine::ProceedControllersPre( float timeInterval )
{
	//todo:	przerobiæ na wersjê wielow¹tkow¹
	for ( unsigned int i = 0; i < m_preControlledObjects.size(); ++i )
		if ( m_preControlledObjects[i]->GetController() != nullptr )
			m_preControlledObjects[i]->GetController()->ControlObjectPre( m_preControlledObjects[i], m_globalState );
}

/**@brief Wywo³uje kontrolery na obs³ugiwanych obiektach.

Funkcja wywo³ywana po MovementEngine::ProceedMovement.
@see @ref Controllers*/
void ControllersEngine::ProceedControllersPost( float timeInterval )
{
	//todo:	przerobiæ na wersjê wielow¹tkow¹
	for ( unsigned int i = 0; i < m_postControlledObjects.size( ); ++i )
		if ( m_postControlledObjects[i]->GetController( ) != nullptr )
			m_postControlledObjects[i]->GetController( )->ControlObjectPost( m_postControlledObjects[i], m_globalState );
}

/**@brief Aktualizuje stan m_globalState.*/
void ControllersEngine::SingleThreadedUpdatePhase( float timeInterval )
{
	UpdateGlobalState( timeInterval );
}


/**@brief Dodaje podany obiekt do kontrolerów wywo³ywanych przed przesuniêciem obiektów.*/
bool ControllersEngine::AddPreControlled( DynamicActor* object )
{
	if ( object == nullptr )
		return false;

	m_preControlledObjects.push_back( object );
	return true;
}

/**@brief Dodaje podany obiekt do kontrolerów wywo³ywanych po przesuniêciu obiektów.*/
bool ControllersEngine::AddPostControlled( DynamicActor* object )
{
	if ( object == nullptr )
		return false;

	m_postControlledObjects.push_back( object );
	return true;
}

/**@brief */
IControllersState* ControllersEngine::GetGlobalState()
{
	return m_globalState;
}

//====================================================================================//
//			Private	
//====================================================================================//

void		ControllersEngine::UpdateGlobalState				( float timeInterval )
{
	m_globalState->Camera = engine->Rendering.GetCurrentCamera()->GetCameraData();
}

