#include "EngineCore/stdafx.h"
#include "LightBase.h"

#include "EngineCore/DisplayEngine/LightModule.h"

#include "Common/MemoryLeaks.h"


RTTR_REGISTRATION
{
	rttr::registration::class_< LightBase >( "LightBase" )
		.property( "Color", &LightBase::m_color )
			(	
				rttr::metadata( MetaDataType::Category, "Light" ),
				rttr::policy::prop::BindAsPtr()
			)
		.property( "Intensity", &LightBase::m_intensity )
			(	rttr::metadata( MetaDataType::Category, "Light" )	);
}


using namespace DirectX;

/**@brief */
LightBase::LightBase( LightType type )
	:	m_type( type )
	,	m_intensity( 1.0f )
	,	m_color( 1.0f, 1.0f, 1.0f )
{}






// ================================ //
//
void		LightBase::SetLightColor		( DirectX::XMFLOAT3& color )
{
	m_color = color;
}

// ================================ //
//
void		LightBase::SetLightColor		( DirectX::XMVECTOR color )
{
	XMStoreFloat3( &m_color, color );
}

// ================================ //
//
void		LightBase::SetLightIntensity	( float intensity )
{
	m_intensity = intensity;
}

/**@brief Sets direction.

Function computes orinetation which will give wanted direction of light.*/
void		LightBase::SetLightDirection	( XMFLOAT3& direction )
{
	XMVECTOR newDirection = XMLoadFloat3( &direction );
	SetLightDirection( newDirection );
}

/**@copydoc LightBase::SetLightDirection*/
void		LightBase::SetLightDirection	( DirectX::XMVECTOR direction )
{
	XMVECTOR defaultOrient = LightModule::DefaultDirectionVec();
	XMVECTOR newDirection = XMVector3Normalize( direction );

	XMVECTOR rotationAxis = XMVector3Normalize( XMVector3Cross( defaultOrient, newDirection ) );
	XMVECTOR angle = XMVector3AngleBetweenNormals( defaultOrient, newDirection );

	XMVECTOR newOrientation = XMQuaternionRotationNormal( rotationAxis, XMVectorGetX( angle ) );

	/// @todo Maybe this should be done by signal ??
	TeleportOrientation( newOrientation );
}
