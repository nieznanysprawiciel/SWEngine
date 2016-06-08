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
const std::vector<ActorData>&		ActorsManager::GetAllActors() const
{
	return m_objectList;
}

/**@brief Zwraca mapê nazw aktorów i wskaŸników na nich.*/
const std::map< std::string, ActorBase* > ActorsManager::GetActorsNames() const
{
	return m_actorNamesMap;
}

/**@brief Wyszukuje aktora.

@todo Byæ mo¿e przeszukiwanie listy aktorów od ty³u bêdzie wydajniejsze.*/
ActorData*	ActorsManager::FindActor( ActorBase* actor )
{
	for( auto& actorData : m_objectList )
	{
		if( actorData.first == actor )
			return &actorData;
	}
	return nullptr;
}

/**@brief Znajduje dane aktora.*/
ActorData* ActorsManager::FindActorByName( const std::string& name )
{
	auto actor = GetActorByName( name );
	if( actor )
		return FindActor( actor );
	return nullptr;
}

/**@brief Zwraca wskaŸnik na aktora o podanej nazwie.*/
ActorBase* ActorsManager::GetActorByName( const std::string& name )
{
	auto iter = m_actorNamesMap.find( name );
	if( iter != m_actorNamesMap.end() )
		return iter->second;
	return nullptr;
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
