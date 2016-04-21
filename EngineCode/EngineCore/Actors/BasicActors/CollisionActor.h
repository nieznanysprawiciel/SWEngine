#pragma once


#include "StaticActor.h"



/**@brief Klasa bazowa dla obiektów zdolnych do kolizji.*/
class CollisionActor : public StaticActor
{
	RTTR_ENABLE( StaticActor )

public:
	static ActorBase*			Create()	{ return new CollisionActor; }
};

