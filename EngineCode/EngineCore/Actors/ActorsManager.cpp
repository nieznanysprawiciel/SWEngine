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

const std::string		ActorsManager::ACTORS_ARRAY_STRING			= "Actors";
const std::string		ActorsManager::ACTOR_ARRAY_ELEMENT_STRING	= "Actor";
const std::string		ActorsManager::ACTOR_INFO_STRING			= "ActorInfo";
const std::string		ActorsManager::ACTOR_OBJECT_NAME			= "ActorName";

/**@brief Serializuje aktorów.*/
void								ActorsManager::Serialize( ISerializer* ser )
{
	ser->EnterArray( ACTORS_ARRAY_STRING );

	for( auto& obj : m_objectList )
	{
		ser->EnterObject( ACTOR_ARRAY_ELEMENT_STRING );
		obj.first->Serialize( ser );
		ser->Exit();
	}

	ser->Exit();
}

/**@brief Deserializuje aktorów.

@todo Nazwy u¿ywane w serializacji s¹ wpisane na sztywno i s¹ u¿ywane równie¿ w EditorPlugin.
Trzeba to ujednoliciæ.*/
void								ActorsManager::Deserialize( IDeserializer* deser )
{
	if( deser->EnterArray( ActorsManager::ACTORS_ARRAY_STRING ) )
	{
		if( deser->FirstElement() )
		{
			do
			{
				auto actorName = deser->GetAttribute( ActorsManager::ACTOR_OBJECT_NAME, "" );

				if( deser->EnterObject( ActorsManager::ACTOR_INFO_STRING ) )
				{
					/// @todo ActorInfo
					ActorInfo info;
					if( deser->GetAttribute( "EnableDisplay", false ) )
						info.actorFlags = ActorInfoFlag::EnableDisplay;
					
					bool actorObjectExists = deser->NextElement();
					assert( actorObjectExists );

					/// @todo Actors info passed in parameter isn't used. Repair it.
					auto newActor = CreateActor( deser->GetName() );
					assert( newActor );
					UpdateActor( newActor, info );

					newActor->Deserialize( deser );

					if( strcmp( actorName, "" ) )
						m_actorNamesMap.insert( std::make_pair( actorName, newActor ) );

					deser->Exit();	// ActorsManager::ACTOR_INFO_STRING
				}

			} while( deser->NextElement() );
		}

		deser->Exit();	// ACTORS_ARRAY_STRING
	}
}
