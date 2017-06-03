/**
@file LightBase.cpp
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/


#include "EngineCore/stdafx.h"
#include "LightBase.h"

#include "EngineCore/DisplayEngine/LightModule.h"

#include "swCommonLib/Common/MemoryLeaks.h"


RTTR_REGISTRATION
{
	rttr::registration::class_< sw::LightBase >( "LightBase" )
		.property( "Color", &sw::LightBase::m_color )
			(	
				rttr::metadata( MetaDataType::Category, "Light" ),
				rttr::policy::prop::bind_as_ptr
			)
		.property( "Intensity", &sw::LightBase::m_intensity )
			(	rttr::metadata( MetaDataType::Category, "Light" )	);
}


using namespace DirectX;


namespace sw
{


/**@brief */
LightBase::LightBase( LightType type )
	: m_type( type )
	, m_intensity( 1.0f )
	, m_color( 1.0f, 1.0f, 1.0f )
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

}	// sw

