#include "EngineCore/stdafx.h"
/**
@file TransformComponent.cpp
@author nieznanysprawiciel
@copyright File is part of graphic engine SWEngine.
*/

#include "TransformComponent.h"


RTTR_REGISTRATION
{
	rttr::registration::class_< sw::TransformComponent >( "TransformComponent" )
		.property( "Position", &sw::TransformComponent::m_position ) BIND_AS_PTR
		.property( "Orientation", &sw::TransformComponent::m_orientation ) BIND_AS_PTR
		.property( "Scale", &sw::TransformComponent::m_scale );
}

using namespace DirectX;


namespace sw
{



// ================================ //
//
TransformComponent::TransformComponent()
	: m_position( XMFLOAT3( 0.0f, 0.0f, 0.0f ) )
	, m_orientation( XMFLOAT4( 0.0f, 0.0f, 0.0f, 1.0f ) )
	, m_scale( 1.0f )
{}

// ================================ //
//
TransformComponent::~TransformComponent()
{}


/**@brief Sets position.
If you need to load XMVECTOR from XMFLOAT3, better use normal SetPosition function.*/
void		TransformComponent::SetPositionXM		( DirectX::XMVECTOR position )
{
	XMStoreFloat3( &m_position, position );
}

// ================================ //
//
void		TransformComponent::SetPosition			( const DirectX::XMFLOAT3& position )
{
	m_position = position;
}

/**@brief Sets orientation.
If you need to load XMVECTOR from XMFLOAT4, better use normal SetOrientation function.*/
void		TransformComponent::SetOrientationXM	( DirectX::XMVECTOR orientation )
{
	XMStoreFloat4( &m_orientation, orientation );
}

// ================================ //
//
void		TransformComponent::SetOrientation		( const DirectX::XMFLOAT4& orientation )
{
	m_orientation = orientation;
}


// ================================ //
//
const DirectX::XMFLOAT3&		TransformComponent::GetPosition() const
{
	return m_position;
}

// ================================ //
//
const DirectX::XMFLOAT4&		TransformComponent::GetOrientation() const
{
	return m_orientation;
}

// ================================ //
//
DirectX::XMVECTOR				TransformComponent::GetPositionXM() const
{
	XMVECTOR pos;
	pos = XMLoadFloat3( &m_position );
	return pos;
}

// ================================ //
//
DirectX::XMVECTOR				TransformComponent::GetOrientationXM() const
{
	XMVECTOR orient;
	orient = XMLoadFloat4( &m_orientation );
	return orient;
}

// ================================ //
//
float		TransformComponent::GetRotationX() const
{
	assert( false );
	return 0.0f;
}

// ================================ //
//
float		TransformComponent::GetRotationY() const
{
	assert( false );
	return 0.0f;
}

// ================================ //
//
float		TransformComponent::GetRotationZ() const
{
	assert( false );
	return 0.0f;
}

// ================================ //
//
float		TransformComponent::GetYaw() const
{
	assert( false );
	return 0.0f;
}

// ================================ //
//
float		TransformComponent::GetPitch() const
{
	assert( false );
	return 0.0f;
}

// ================================ //
//
float		TransformComponent::GetRoll() const
{
	assert( false );
	return 0.0f;
}

// ================================ //
//
void		TransformComponent::SetYaw( float angle )
{
	assert( false );
}

// ================================ //
//
void		TransformComponent::SetPitch( float angle )
{
	assert( false );
}

// ================================ //
//
void		TransformComponent::SetRoll( float angle )
{
	assert( false );
}

}	// sw