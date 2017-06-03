/**
@file Math.cpp
@author nieznanysprawiciel
@copyright File is part of graphic engine SWEngine.
*/
#include "EngineCore/stdafx.h"

#include "Math.h"

using namespace DirectX;


namespace sw
{


// ================================ //
//
float				Math::OrientedAngle		( DirectX::XMVECTOR first, DirectX::XMVECTOR second, DirectX::XMVECTOR planeNormal )
{
	float angle = XMVectorGetX( XMVector3AngleBetweenVectors( first, second ) );
	XMVECTOR cross = XMVector3Cross( first, second );

	auto dot = XMVectorGetX( XMVector3Dot( cross, planeNormal ) );
	if( dot < 0.0 )
	{
		angle = -angle;
	}

	return angle;
}

// ================================ //
//
float				Math::ToDegrees			( float value )
{
	return XMConvertToDegrees( value );
}

// ================================ //
//
float				Math::ToRadians			( float value )
{
	return XMConvertToRadians( value );
}



}	// sw

