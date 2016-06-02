#include "EngineCore/stdafx.h"
#include "Intersections.h"

#include "EngineCore/MainEngine/EngineInterface.h"
#include "EngineCore/Actors/BasicActors/CameraActor.h"


using namespace DirectX;


/**@brief Wylicza wektor kierunku promienia wyznaczanego przez klikniêcie mysz¹.

@return Zwracany wektor jest ju¿ znormalizowany.*/
XMVECTOR		Intersections::ComputeMouseRayDirection		( EngineInterface* engine, CameraData& camera, float mouseX, float mouseY )
{
	float halfWindowX = (float)engine->Rendering.GetWindowWidth() / 2.0f;
	float halfWindowY = (float)engine->Rendering.GetWindowHeight() / 2.0f;

	float depth = static_cast< float >( halfWindowY / tan( DirectX::XMConvertToRadians( camera.Fov ) / 2.0f ) );

	XMVECTOR cameraSpaceDir = XMVectorSet( mouseX - halfWindowX, halfWindowY - mouseY, -depth, 0.0f );

	return XMVector3Normalize( XMVector3Rotate( cameraSpaceDir, camera.GetOrientation() ) );
}

/**@brief Wylicza punkt przeciêcia z p³aszczyzn¹ XY o wspó³rzêdnej z równej zCoord.*/
XMVECTOR		Intersections::PlaneXZIntersection				( XMVECTOR rayDir, XMVECTOR rayPoint, float zCoord )
{
	XMVECTOR planeXY = XMVectorSet( 0.0f, 1.0f, 0.0f, -zCoord );
	return XMPlaneIntersectLine( planeXY, rayPoint, XMVectorAdd( rayPoint, rayDir ) );
}
