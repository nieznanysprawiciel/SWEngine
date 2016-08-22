#pragma once

#include "StaticActor.h"


/**@brief */
class TriggerActor : public StaticActor
{
	RTTR_ENABLE( StaticActor )
	RTTR_REGISTRATION_FRIEND
private:
protected:
public:
	explicit TriggerActor();
	virtual ~TriggerActor();

	static ActorBase*		Create()	{ return new TriggerActor; }
};
