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

	inline ActorFactory&		GetActorFactory()	{ return m_actorFactory; }
};

