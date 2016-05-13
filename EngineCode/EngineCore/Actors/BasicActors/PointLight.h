#pragma once


#include "LightBase.h"


/**@brief */
class PointLight : public LightBase
{
	RTTR_ENABLE( LightBase )
	RTTR_REGISTRATION_FRIEND
private:
protected:
	
	float			m_radius;

public:
	explicit PointLight();
	virtual ~PointLight() = default;

	static ActorBase*		Create()	{ return new PointLight; }

protected:
	explicit PointLight	( LightType type );
};

