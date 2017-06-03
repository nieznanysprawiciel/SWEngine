/**
@file ActorsManager.cpp
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/


#include "EngineCore/stdafx.h"
#include "ActorsManager.h"

#include "swCommonLib/Serialization/Deserializer.h"
#include "swCommonLib/Serialization/Serializer.h"

#include "EngineCore/EngineHelpers/ActorsCommonFunctions.h"


namespace sw
{



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

/**@brief Usuwa wszystkich aktor�w.*/
void ActorsManager::RemoveAllActors()
{
	m_actorNamesMap.clear();

	for( Size i = 0; i < m_objectList.size(); ++i )
		delete m_objectList[ i ].first;

	m_objectList.clear();
}

/**@brief Usuwa aktor�w z modu�u.

Poniewa� ActorsManager jest w�a�cicielem aktor�w, to usuni�cie aktora, wi��e si�
ze zwolnieniem pami�ci.

@attention Jaka� zewn�trzna funkcja musi si� zatroszczy� o zwolnienie aktor�w z pozosta�ych miejsc w silniku.*/
void ActorsManager::RemoveActor( ActorBase* actor )
{
	// Je�eli aktor mia� nazw� to kasujemy wpis.
	for( auto iter = m_actorNamesMap.begin(); iter != m_actorNamesMap.end(); iter++ )
	{
		if( iter->second == actor )
		{
			m_actorNamesMap.erase( iter );
			break;
		}
	}

	// Kasujemy samego aktora.
	for( int i = (int)m_objectList.size() - 1; i > 0; --i )
	{
		if( m_objectList[ i ].first == actor )
		{
			delete m_objectList[ i ].first;
			m_objectList.erase( m_objectList.begin() + i );
			break;
		}
	}
}

/**@brief Aktualizuje */
void ActorsManager::UpdateActor		( ActorBase* actor, ActorInfo actorModules )
{
	auto actorData = FindActor( actor );

	assert( actorData );

	actorData->second |= actorModules;
}

/**@brief Nadaje nazw� aktorowi.

@return Zwraca false, je�eli aktor o podanej nazwie istnieje.*/
bool ActorsManager::NameActor		( ActorBase* actor, const std::string& name )
{
	auto iter = m_actorNamesMap.find( name );
	if( iter != m_actorNamesMap.end() )
		return false;

	m_actorNamesMap.insert( std::make_pair( name, actor ) );
	return true;
}

/**@brief Pobiera dane o wszystkich aktorach.

@return Zwraca referencj� na wektor @ref ActorData.*/
const std::vector<ActorData>&		ActorsManager::GetAllActors() const
{
	return m_objectList;
}

/**@brief Zwraca map� nazw aktor�w i wska�nik�w na nich.*/
const std::map< std::string, ActorBase* >& ActorsManager::GetActorsNames() const
{
	return m_actorNamesMap;
}

/**@brief Wyszukuje aktora.

@todo By� mo�e przeszukiwanie listy aktor�w od ty�u b�dzie wydajniejsze.*/
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

/**@brief Zwraca wska�nik na aktora o podanej nazwie.*/
ActorBase* ActorsManager::GetActorByName( const std::string& name )
{
	auto iter = m_actorNamesMap.find( name );
	if( iter != m_actorNamesMap.end() )
		return iter->second;
	return nullptr;
}


}	// sw
