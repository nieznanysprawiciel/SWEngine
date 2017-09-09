#pragma once
/**
@file Intersections.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/


#include <DirectXMath.h>


namespace sw
{


struct CameraData;
class EngineInterface;


class Intersections
{
public:

	static DirectX::XMVECTOR		ComputeMouseRayDirection	( EngineInterface* engine, CameraData& camera, float mouseX, float mouseY );
	static DirectX::XMVECTOR		ComputeMouseRayDirection	( float windowWidth, float windowHeight, CameraData& camera, float mouseX, float mouseY );

	static DirectX::XMVECTOR		PlaneXZIntersection			( DirectX::XMVECTOR rayDir, DirectX::XMVECTOR rayPoint, float zCoord );
};

}	// sw
