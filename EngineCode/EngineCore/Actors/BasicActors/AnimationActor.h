#pragma once

#include "PhysicalActor.h"


class AnimationActor : public PhysicalActor
{
	friend class DisplayEngine;

	RTTR_ENABLE( PhysicalActor )
protected:

public:

	static EngineObject*		Create()	{ return new AnimationActor; }
};


