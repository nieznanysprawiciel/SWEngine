#pragma once

#include "CollisionActor.h"


/**@brief */
class TriggerActor : public CollisionActor
{
	RTTR_ENABLE( CollisionActor )
	RTTR_REGISTRATION_FRIEND
private:
protected:
public:
	explicit TriggerActor();
	virtual ~TriggerActor();

	static ActorBase*		Create()	{ return new TriggerActor; }
};
