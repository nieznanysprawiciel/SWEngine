#pragma once


#include "LightBase.h"


/**@brief Point light class.

@ingroup Actors
@ingroup Lights*/
class PointLight : public LightBase
{
	RTTR_ENABLE( LightBase )
	RTTR_REGISTRATION_FRIEND
private:
protected:
	
	float			m_clampRadius;
	float			m_constAttenuation;
	float			m_linearAttenuation;
	float			m_quadraticAttentuation;

public:
	explicit PointLight();
	virtual ~PointLight() = default;

	static ActorBase*		Create()	{ return new PointLight; }

protected:
	explicit PointLight	( LightType type );

public:
	float		GetClampRadius			()		{ return m_clampRadius; }
	float		GetConstantAttenuation	()		{ return m_constAttenuation; }
	float		GetLinearAttenuation	()		{ return m_linearAttenuation; }
	float		GetQuadraticAttenuation	()		{ return m_quadraticAttentuation; }
};

