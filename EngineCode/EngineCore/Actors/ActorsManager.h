#pragma once

#include "ActorFactory.h"
#include "ActorInfo.h"

class Engine;



/**@brief Modu³ do zarz¹dzania aktorami.
@ingroup EngineCore*/
class ActorsManager
{
private:
	ActorFactory				m_actorFactory;

	std::vector< ActorData >	m_objectList;

protected:
public:
	ActorsManager( Engine* engine );
	~ActorsManager();

	inline ActorFactory&									GetActorFactory	()	{ return m_actorFactory; }

	template< typename Type = EngineObject >	Type*		CreateActor		( const std::string& name, ActorInfo actorModules );
	template< typename Type = EngineObject >	Type*		CreateActor		( ActorType id, ActorInfo actorModules );

	void													UpdateActor		( EngineObject* actor, ActorInfo actorModules );
	const std::vector< ActorData >&							GetAllActors	();

private:

	void													AddActor		( EngineObject* newActor );
	
	template< typename Type = EngineObject >	ActorData*	FindActor( EngineObject* actor );
};


/**@brief Tworzy aktora na podstawie nazwy.*/
template<typename Type>
inline Type* ActorsManager::CreateActor( const std::string& name, ActorInfo actorModules )
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
inline Type* ActorsManager::CreateActor( ActorType id, ActorInfo actorModules )
{
	Type* newActor = m_actorFactory.CreateActor< Type >( id );

	assert( newActor != nullptr );
	if( newActor == nullptr )
		return nullptr;

	AddActor( newActor );

	return newActor;
}

/**@brief Wyszukuje aktora.*/
template< typename Type > ActorData*	ActorsManager::FindActor( EngineObject* actor )
{
	for( auto& actorData : m_objectList )
	{
		if( actorData.first == actor )
			return &actorData;
	}
	return nullptr;
}
