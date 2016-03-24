#pragma once

#include "ActorFactory.h"
#include "ActorInfo.h"

class Engine;

typedef std::pair< Object*, ActorInfo > ActorData;

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

	inline ActorFactory&		GetActorFactory	()	{ return m_actorFactory; }

	template< typename Type = Object >	Type*		CreateActor				( const std::string& name, ActorInfo actorModules );
	template< typename Type = Object >	Type*		CreateActor				( ActorType id, ActorInfo actorModules );

	void						UpdateActor		( Object* actor, ActorInfo actorModules );

private:

	void						AddActor		( Object* newActor );
	
	template< typename Type = Object >	ActorData*	FindActor( Object* actor );
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
template< typename Type > ActorData*	ActorsManager::FindActor( Object* actor )
{
	for( auto& actorData : m_objectList )
	{
		if( actorData.first == actor )
			return &actorData;
	}
	return nullptr;
}
