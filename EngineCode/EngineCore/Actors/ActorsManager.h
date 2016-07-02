#pragma once

#include "ActorFactory.h"
#include "ActorInfo.h"

#include <map>
#include <vector>
#include <string>


class Engine;
class ISerializer;
class IDeserializer;


/**@brief Modu³ do zarz¹dzania aktorami.
@ingroup EngineCore*/
class ActorsManager
{
public:
	static const std::string	ACTORS_ARRAY_STRING;
	static const std::string	ACTOR_ARRAY_ELEMENT_STRING;
	static const std::string	ACTOR_OBJECT_NAME;
	static const std::string	ACTOR_INFO_STRING;

private:
	ActorFactory							m_actorFactory;

	std::vector< ActorData >				m_objectList;
	std::map< std::string, ActorBase* >		m_actorNamesMap;

protected:
public:
	ActorsManager( Engine* engine );
	~ActorsManager();

	inline ActorFactory&									GetActorFactory	()	{ return m_actorFactory; }

	template< typename Type = ActorBase >	Type*			CreateActor		( const std::string& name );
	template< typename Type = ActorBase >	Type*			CreateActor		( ActorType id );

	const std::vector< ActorData >&							GetAllActors	() const;
	const std::map< std::string, ActorBase* >				GetActorsNames	() const;

	ActorData*												FindActor		( ActorBase* actor );
	ActorData*												FindActorByName	( const std::string& name );
	ActorBase*												GetActorByName	( const std::string& name );

	void													UpdateActor		( ActorBase* actor, ActorInfo actorModules );

private:

	void													AddActor		( ActorBase* newActor );
	
public:

	void													Serialize		( ISerializer* ser );
	void													Deserialize		( IDeserializer* deser );
};


/**@brief Tworzy aktora na podstawie nazwy.*/
template<typename Type>
inline Type* ActorsManager::CreateActor( const std::string& name )
{
	Type* newActor = m_actorFactory.CreateActor< Type >( name );

	assert( newActor != nullptr );
	if( newActor == nullptr )
		return nullptr;

	AddActor( newActor );

	return newActor;
}

/**@brief Tworzy aktora na podstawie identyfikatora.*/
template<typename Type>
inline Type* ActorsManager::CreateActor( ActorType id )
{
	Type* newActor = m_actorFactory.CreateActor< Type >( id );

	assert( newActor != nullptr );
	if( newActor == nullptr )
		return nullptr;

	AddActor( newActor );

	return newActor;
}

