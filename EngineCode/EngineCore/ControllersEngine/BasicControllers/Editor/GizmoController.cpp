#include "EngineCore/stdafx.h"
#include "GizmoController.h"

#include "EngineCore/UIEngine/InputAbstractionLayer.h"
#include "EngineCore/UIEngine/StandardAbstractionLayers.h"

#include "EngineCore/Actors/BasicActors/DynamicActor.h"
#include "EngineCore/Actors/BasicActors/CameraActor.h"
#include "EngineCore/MainEngine/EngineInterface.h"

#include "EngineCore/EngineHelpers/Intersections.h"


RTTR_REGISTRATION
{
	rttr::registration::class_< GizmoController >( "GizmoController" );
}

using namespace DirectX;

/**@brief */
GizmoController::GizmoController( InputAbstractionLayerBase* layer )
	:	BaseInputController( layer )
	,	m_followedActor( nullptr )
{}

/**@brief */
GizmoController::~GizmoController()
{}

/**@brief Przemieszcza kontrolowanego aktora.

Aktor gizmo jest przemieszczany dopiero przez funkcjê ControlObjectPost.*/
void GizmoController::ControlObjectPre		( DynamicActor* actor, IControllersState* globalState )
{
	if ( !m_abstractionLayer->IsActive() )
		return;

	auto buttons = m_abstractionLayer->GetButtonsTable();

	if( buttons[ STANDARD_LAYERS::PROTOTYPE_BUTTONS::LEFT_CLICK ] )
	{
		XMVECTOR newPosition;

		CameraData& camera = globalState->Camera;

		XMVECTOR rayDir = ComputeMouseRayDirection( globalState );
		XMVECTOR rayPoint = XMVectorSetW( XMLoadFloat3( &camera.Position ), 1.0f );

		auto actorPosition = actor->GetPosition();
		newPosition = actorPosition;

		bool moveX = buttons[ STANDARD_LAYERS::PROTOTYPE_BUTTONS::ENABLE_MOVE_X ] != 0;
		bool moveY = buttons[ STANDARD_LAYERS::PROTOTYPE_BUTTONS::ENABLE_MOVE_Y ] != 0;
		bool moveZ = buttons[ STANDARD_LAYERS::PROTOTYPE_BUTTONS::ENABLE_MOVE_Z ] != 0;

		if( moveX || moveZ )
		{
			XMVECTOR intersection = Intersections::PlaneXZIntersection( rayDir, rayPoint, -XMVectorGetY( actorPosition ) );

			if( moveX && !isnan( XMVectorGetX( intersection ) ) )
				newPosition = XMVectorSetX( newPosition, XMVectorGetX( intersection ) );

			if( moveZ && !isnan( XMVectorGetZ( intersection ) ) )
				newPosition = XMVectorSetZ( newPosition, XMVectorGetZ( intersection ) );
		}

		if( moveY )
		{
			XMVECTOR planeY = XMPlaneFromPointNormal( actorPosition, XMVectorNegate( rayDir ) );
			XMVECTOR intersection = XMPlaneIntersectLine( planeY, rayPoint, XMVectorAdd( rayPoint, rayDir ) );

			if( !isnan( XMVectorGetY( intersection ) ) )
				newPosition = XMVectorSetY( newPosition, XMVectorGetY( intersection ) );
		}

		/// @todo Zamiast wywo³ywaæ Teleport, nale¿y wys³aæ sygna³.
		m_followedActor->Teleport( newPosition );
	}
}

/**@brief Œledzi podanego aktora.*/
void GizmoController::ControlObjectPost		( DynamicActor* actor, IControllersState* globalState )
{
	if( m_followedActor )
	{
		/// @todo W zasadzie pobieranie pozycji aktora jest nielegalne z punktu widzenia wielow¹tkowoœci.
		/// Trzeba jednak wymyœleæ coœ, ¿eby mo¿na by³o jakoœ dostawaæ dane od innych aktorów z poprzedniej klatki.
		auto position = m_followedActor->GetPosition();
		actor->Teleport( position );
	}
}

/**@brief Wylicza wektor kierunku promienia wyznaczanego przez klikniêcie mysz¹.

Wektor jest we wspó³rzêdnych lokalnych kamery, tzn przy za³o¿eniu, ¿e kamera jest skierowana w kierunku
(0.0, 0.0, -1.0). Wymaga on przemno¿enia przez kwaternion orientacji kamery.
Wektor nie jest znormalizowany.*/
DirectX::XMVECTOR GizmoController::ComputeMouseRayDirection( IControllersState* globalState )
{
	float mouseX = (float)m_abstractionLayer->GetMouseX();
	float mouseY = (float)m_abstractionLayer->GetMouseY();

	return Intersections::ComputeMouseRayDirection( globalState->Engine, globalState->Camera, mouseX, mouseY );
}

