#pragma once


#include "StaticActor.h"



/**@brief Klasa bazowa dla obiekt�w zdolnych do kolizji.*/
class CollisionActor : public StaticActor
{
	RTTR_ENABLE( StaticActor )

public:
	static Object*			Create()	{ return new CollisionActor; }
};
