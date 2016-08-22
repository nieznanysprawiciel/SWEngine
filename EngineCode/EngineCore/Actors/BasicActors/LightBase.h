#pragma once

#include "DynamicActor.h"


enum class LightType
{
	PointLight,
	SpotLight,
	DirectionalLight
};


/**@brief Klasa bazowa dla œwiate³.

Klasa dziedziczy po CollisionActor, ¿eby mo¿na by³o wyliczyæ kolizje i eliminowaæ nieprzydatne œwiat³a.*/
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


