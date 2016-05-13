#pragma once


#include "PointLight.h"


/**@brief */
class SpotLight	: public PointLight
{
	RTTR_ENABLE( PointLight )
	RTTR_REGISTRATION_FRIEND
private:
protected:

	float			m_angle;

public:
	explicit SpotLight();
	virtual ~SpotLight() = default;

	static ActorBase*		Create()	{ return new SpotLight; }
};

