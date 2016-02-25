#include "EngineCore/stdafx.h"
#include "ActorsManager.h"



/**@brief */
ActorsManager::ActorsManager( Engine* engine )
{}

/**@brief */
ActorsManager::~ActorsManager()
{
	for( auto& actor : m_objectList )
		delete actor.first;
}

/**@brief Dodaje aktora do ActorsManagera.*/
void ActorsManager::AddActor( Object* newActor, ActorInfo actorModules )
{
	assert( newActor != nullptr );
	m_objectList.push_back( std::make_pair( newActor, actorModules ) );
}
