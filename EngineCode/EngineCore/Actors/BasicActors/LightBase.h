#pragma once
/**
@file LightBase.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/


#include "DynamicActor.h"


namespace sw
{

/**@brief Type of light.
@ingroup Lights*/
enum class LightType : int32
{
	PointLight = 0,
	SpotLight,
	DirectionalLight
};


/**@brief Klasa bazowa dla �wiate�.

@ingroup Actors
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

	LightType			GetLightType	() { return m_type; }

	DirectX::XMFLOAT3	GetColor		() { return m_color; }
	float				GetIntensity	() { return m_intensity; }

public:
	///< @todo Maybe these functions should be invoked as signals??
	void				SetLightColor		( DirectX::XMVECTOR color );
	void				SetLightColor		( DirectX::XMFLOAT3& color );
	void				SetLightIntensity	( float intensity );
	void				SetLightDirection	( DirectX::XMFLOAT3& direction );
	void				SetLightDirection	( DirectX::XMVECTOR direction );
};


}	// sw
