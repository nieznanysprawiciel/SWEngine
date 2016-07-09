#include "EngineCore/stdafx.h"
#include "ControllersEngine.h"

#include "EngineCore/MainEngine/Engine.h"
#include "EngineCore/Actors/ActorObjects.h"

#include "EngineCore/ControllersEngine/IControllersState.h"

#include "EngineCore/EngineHelpers/ActorsCommonFunctions.h"

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



/**@brief Wywo�uje kontrolery na obs�ugiwanych obiektach.

Funkcja wywo�ywana przed MovementEngine::ProceedMovement.
@see @ref Controllers*/
void ControllersEngine::ProceedControllersPre( float timeInterval )
{
	//todo:	przerobi� na wersj� wielow�tkow�
	for ( unsigned int i = 0; i < m_preControlledObjects.size(); ++i )
		if ( m_preControlledObjects[i]->GetController() != nullptr )
			m_preControlledObjects[i]->GetController()->ControlObjectPre( m_preControlledObjects[i], m_globalState );
}

/**@brief Wywo�uje kontrolery na obs�ugiwanych obiektach.

Funkcja wywo�ywana po MovementEngine::ProceedMovement.
@see @ref Controllers*/
void ControllersEngine::ProceedControllersPost( float timeInterval )
{
	//todo:	przerobi� na wersj� wielow�tkow�
	for ( unsigned int i = 0; i < m_postControlledObjects.size( ); ++i )
		if ( m_postControlledObjects[i]->GetController( ) != nullptr )
			m_postControlledObjects[i]->GetController( )->ControlObjectPost( m_postControlledObjects[i], m_globalState );
}

/**@brief Aktualizuje stan m_globalState.*/
void ControllersEngine::SingleThreadedUpdatePhase( float timeInterval )
{
	UpdateGlobalState( timeInterval );
}


/**@brief Dodaje podany obiekt do kontroler�w wywo�ywanych przed przesuni�ciem obiekt�w.*/
bool ControllersEngine::AddPreControlled( DynamicActor* actor )
{
	if ( actor == nullptr )
		return false;

	m_preControlledObjects.push_back( actor );
	return true;
}

/**@brief Dodaje podany obiekt do kontroler�w wywo�ywanych po przesuni�ciu obiekt�w.*/
bool ControllersEngine::AddPostControlled( DynamicActor* actor )
{
	if ( actor == nullptr )
		return false;

	m_postControlledObjects.push_back( actor );
	return true;
}

/**@brief Usuwa aktora z tablicy obiekt�w pre kontrolowanych.*/
void ControllersEngine::RemovePreCtrlActor( ActorBase* actor )
{
	ActorsCommonFunctions::RemoveActor( m_preControlledObjects, static_cast< DynamicActor* >( actor ) );
}

/**@brief Usuwa aktora z tablicy obiekt�w post kontrolowanych.*/
void ControllersEngine::RemovePostCtrlActor( ActorBase* actor )
{
	ActorsCommonFunctions::RemoveActor( m_postControlledObjects, static_cast< DynamicActor* >( actor ) );
}

/**@brief Usuwa aktora z modu�u.*/
void ControllersEngine::RemoveActor( ActorBase* actor )
{
	RemovePreCtrlActor( actor );
	RemovePostCtrlActor( actor );
}

/**@brief Usuwa wszystkich aktor�w.*/
void ControllersEngine::RemoveAllActors()
{
	m_postControlledObjects.clear();
	m_preControlledObjects.clear();
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

