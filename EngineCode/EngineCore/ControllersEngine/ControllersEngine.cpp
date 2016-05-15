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



//funkcja wywo³ywana przez klasê Engine w ka¿dym obiegu g³ównej petli programu
//parametrem jest czas który up³yn¹³ od ostatniego wywo³ania
void ControllersEngine::ProceedControllersPre(float time_interval)
{
	//todo:	przerobiæ na wersjê wielow¹tkow¹
	for ( unsigned int i = 0; i < m_preControlledObjects.size(); ++i )
		if ( m_preControlledObjects[i]->GetController() != nullptr )
			m_preControlledObjects[i]->GetController()->ControlObjectPre( m_preControlledObjects[i] );
}


void ControllersEngine::ProceedControllersPost( float time_interval )
{
	//todo:	przerobiæ na wersjê wielow¹tkow¹
	for ( unsigned int i = 0; i < m_postControlledObjects.size( ); ++i )
		if ( m_postControlledObjects[i]->GetController( ) != nullptr )
			m_postControlledObjects[i]->GetController( )->ControlObjectPost( m_postControlledObjects[i] );
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
