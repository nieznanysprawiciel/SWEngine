#pragma once

#include "DynamicActor.h"




class PhysicalActor : public DynamicActor
{
	RTTR_REGISTRATION_FRIEND
	RTTR_ENABLE( DynamicActor )
protected:

	float			mass;

public:

	PhysicalActor();

	void Pulse();

	static ActorBase*		Create()	{ return new PhysicalActor; }
};

