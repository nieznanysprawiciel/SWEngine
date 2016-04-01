#pragma once

#include "PhysicalActor.h"


class AnimationActor : public PhysicalActor
{
	friend class DisplayEngine;

	RTTR_ENABLE( PhysicalActor )
protected:

public:

	static Object*		Create()	{ return new AnimationActor; }
};


