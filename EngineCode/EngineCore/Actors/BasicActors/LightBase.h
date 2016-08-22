#pragma once

#include "DynamicActor.h"


enum class LightType
{
	PointLight,
	SpotLight,
	DirectionalLight
};


/**@brief Klasa bazowa dla �wiate�.

Klasa dziedziczy po CollisionActor, �eby mo�na by�o wyliczy� kolizje i eliminowa� nieprzydatne �wiat�a.*/
class LightBase : public DynamicActor
{
	RTTR_ENABLE( DynamicActor )
	RTTR_REGISTRATION_FRIEND
private:
	LightType			m_type;
protected:

	DirectX::XMFLOAT3		m_color;
	float					m_intensity;

public:
	explicit LightBase	( LightType type );
	virtual ~LightBase	() = default;
};


