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
void ActorsManager::AddActor( ActorBase* newActor )
{
	assert( newActor != nullptr );
	ActorInfo actorInfo;			// Wszystkie warto�ci false.
	m_objectList.push_back( std::make_pair( newActor, actorInfo ) );
}

/**@brief Aktualizuje */
void ActorsManager::UpdateActor		( ActorBase* actor, ActorInfo actorModules )
{
	auto actorData = FindActor( actor );

	assert( actorData );

	actorData->second |= actorModules;
}

/**@brief Pobiera dane o wszystkich aktorach.

@return Zwraca referencj� na wektor @ref ActorData.*/
const std::vector<ActorData>&		ActorsManager::GetAllActors()
{
	return m_objectList;
}
