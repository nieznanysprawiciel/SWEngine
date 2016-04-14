#pragma once

#include "DynamicActor.h"




class PhysicalActor : public DynamicActor
{
	RTTR_REGISTRATION_FRIEND
	RTTR_ENABLE( DynamicActor )
protected:

	float			mass;
	int				testInt;

public:

	PhysicalActor();

	void Pulse();

	static EngineObject*		Create()	{ return new PhysicalActor; }
};

