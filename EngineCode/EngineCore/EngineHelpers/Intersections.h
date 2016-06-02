#pragma once

#include <DirectXMath.h>

struct CameraData;
class EngineInterface;


class Intersections
{
public:

	static DirectX::XMVECTOR		ComputeMouseRayDirection	( EngineInterface* engine, CameraData& camera, float mouseX, float mouseY );

	static DirectX::XMVECTOR		PlaneXZIntersection			( DirectX::XMVECTOR rayDir, DirectX::XMVECTOR rayPoint, float zCoord );
};

