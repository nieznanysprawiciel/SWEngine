#pragma once

#include "DynamicActor.h"




class PhysicalActor : public DynamicActor
{
	RTTR_ENABLE( DynamicActor )
protected:

	float			mass;

public:

	PhysicalActor();

	void Pulse();

	static Object*		Create()	{ return new PhysicalActor; }
};

