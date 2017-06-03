#pragma once
/**
@file TriggerActor.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/


#include "StaticActor.h"



namespace sw
{

/**@brief

@ingroup Actors*/
class TriggerActor : public StaticActor
{
	RTTR_ENABLE( StaticActor )
		RTTR_REGISTRATION_FRIEND
private:
protected:
public:
	explicit TriggerActor();
	virtual ~TriggerActor();

	static ActorBase*		Create() { return new TriggerActor; }
};

}	// sw

