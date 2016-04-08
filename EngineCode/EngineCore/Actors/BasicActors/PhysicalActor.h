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

	static EngineObject*		Create()	{ return new PhysicalActor; }
};

