#include "EngineCore/stdafx.h"
#include "ActorsManager.h"

#include "Common/Serialization/Deserializer.h"
#include "Common/Serialization/Serializer.h"


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
	ActorInfo actorInfo;			// Wszystkie wartoœci false.
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

@return Zwraca referencjê na wektor @ref ActorData.*/
const std::vector<ActorData>&		ActorsManager::GetAllActors()
{
	return m_objectList;
}

//====================================================================================//
//			Serialization	
//====================================================================================//

const std::string		ACTORS_ARRAY_NAME		= "Actors";
const std::string		ACTOR_INSTANCE_NAME		= "Actor";

/**@brief Serializuje aktorów.*/
void								ActorsManager::Serialize( ISerializer* ser )
{
	ser->EnterArray( ACTORS_ARRAY_NAME );

	for( auto& obj : m_objectList )
	{
		ser->EnterObject( ACTOR_INSTANCE_NAME );
		obj.first->Serialize( ser );
		ser->Exit();
	}

	ser->Exit();
}

/**@brief Deserializuje aktorów.*/
void								ActorsManager::Deserialize( IDeserializer* deser )
{

}
