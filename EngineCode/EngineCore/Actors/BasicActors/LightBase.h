#pragma once

#include "DynamicActor.h"


enum class LightType : int32
{
	PointLight = 0,
	SpotLight,
	DirectionalLight
};


/**@brief Klasa bazowa dla œwiate³.
@ingroup Lights*/
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

	LightType			GetLightType	()		{ return m_type; }

	DirectX::XMFLOAT3	GetColor		()		{ return m_color; }
	float				GetIntensity	()		{ return m_intensity; }
};


