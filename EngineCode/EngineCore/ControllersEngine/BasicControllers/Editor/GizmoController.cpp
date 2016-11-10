#include "EngineCore/stdafx.h"
#include "GizmoController.h"

#include "EngineCore/UIEngine/InputAbstractionLayer.h"
#include "EngineCore/UIEngine/StandardAbstractionLayers.h"

#include "EngineCore/Actors/BasicActors/DynamicActor.h"
#include "EngineCore/Actors/BasicActors/CameraActor.h"
#include "EngineCore/MainEngine/EngineInterface.h"

#include "EngineCore/EngineHelpers/Intersections.h"
#include "EngineCore/EngineHelpers/Math.h"


RTTR_REGISTRATION
{
	rttr::registration::class_< GizmoController >( "GizmoController" );
}

using namespace DirectX;

/**@brief */
GizmoController::GizmoController(	InputAbstractionLayerBase* layer,
									ResourcePtr< Model3DFromFile > translateGizmo,
									ResourcePtr< Model3DFromFile > rotateGizmo,
									ResourcePtr< Model3DFromFile > scaleGizmo )
	:	BaseInputController( layer )
	,	m_followedActor( nullptr )
	,	m_controlledActor( nullptr )
	,	m_translateGizmo( translateGizmo )
	,	m_rotateGizmo( rotateGizmo )
	,	m_scaleGizmo( scaleGizmo )
	,	m_operation( GizmoController::Operation::Translate )
{
	m_rotationOp.Started = false;
	m_rotationOp.UseStep = false;
	m_rotationOp.StepSize = 10;
}

/**@brief */
GizmoController::~GizmoController()
{}


// ================================ //
//
void		GizmoController::Initialize				( DynamicActor* actor )
{
	m_controlledActor = actor;
	ChangeOperation( GizmoController::Operation::Translate );
}


/**@brief Przemieszcza kontrolowanego aktora.

Aktor gizmo jest przemieszczany dopiero przez funkcjê ControlObjectPost.*/
void		GizmoController::ControlObjectPre		( DynamicActor* actor, IControllersState* globalState )
{
	if ( !m_abstractionLayer->IsActive() || !m_followedActor )
		return;

	if( m_operation == GizmoController::Operation::Translate )
		Translation( actor, globalState );
	else if( m_operation == GizmoController::Operation::Rotate )
		Rotation( actor, globalState );
	else if( m_operation == GizmoController::Operation::Scale )
		Scaling( actor, globalState );
}

/**@brief Œledzi podanego aktora.*/
void		GizmoController::ControlObjectPost		( DynamicActor* actor, IControllersState* globalState )
{
	if( m_followedActor )
	{
		/// @todo W zasadzie pobieranie pozycji aktora jest nielegalne z punktu widzenia wielow¹tkowoœci.
		/// Trzeba jednak wymyœleæ coœ, ¿eby mo¿na by³o jakoœ dostawaæ dane od innych aktorów z poprzedniej klatki.
		auto position = m_followedActor->GetPosition();
		actor->Teleport( position );
	}
}

// ================================ //
//
void							GizmoController::ChangeOperation( Operation op )
{
	if( m_operation == GizmoController::Operation::Rotate )
		EndOpRotation();
	else if( m_operation == GizmoController::Operation::Translate )
		EndOpTranslation();
	else if( m_operation == GizmoController::Operation::Scale )
		EndOpScaling();

	m_operation = op;

	if( m_operation == GizmoController::Operation::Rotate )
		BeginOpRotation();
	else if( m_operation == GizmoController::Operation::Translate )
		BeginOpTranslation();
	else if( m_operation == GizmoController::Operation::Scale )
		BeginOpScaling();
}

// ================================ //
//
void GizmoController::BeginOpRotation()
{
	m_controlledActor->SetModel( m_rotateGizmo.Ptr() );
}

// ================================ //
//
void GizmoController::EndOpRotation()
{
	m_rotationOp.Started = false;
	m_controlledActor->SetModel( nullptr );
}


// ================================ //
//
void GizmoController::BeginOpTranslation()
{
	m_controlledActor->SetModel( m_translateGizmo.Ptr() );
}

// ================================ //
//
void GizmoController::EndOpTranslation()
{
	m_controlledActor->SetModel( nullptr );
}


// ================================ //
//
void GizmoController::BeginOpScaling()
{
	m_controlledActor->SetModel( m_scaleGizmo.Ptr() );
}

// ================================ //
//
void GizmoController::EndOpScaling()
{
	m_controlledActor->SetModel( nullptr );
}



// ================================ //
//
GizmoController::Operation		GizmoController::CheckOperation()
{
	return m_operation;
}

// ================================ //
//
void							GizmoController::SetStepSize( float value )
{
	m_rotationOp.StepSize = value;
}

// ================================ //
//
float							GizmoController::GetStepSize()
{
	return m_rotationOp.StepSize;
}

// ================================ //
//
void							GizmoController::UseStepRotation( bool value )
{
	m_rotationOp.UseStep = value;
}

// ================================ //
//
bool							GizmoController::GetUseStepRotation()
{
	return m_rotationOp.UseStep;
}



// ================================ //
//
void				GizmoController::Rotation		( DynamicActor* actor, IControllersState* globalState )
{
	auto buttons = m_abstractionLayer->GetButtonsTable();

	bool rotX = buttons[ STANDARD_LAYERS::PROTOTYPE_BUTTONS::ENABLE_MOVE_X ].IsPressed();	// Warning 4800
	bool rotY = buttons[ STANDARD_LAYERS::PROTOTYPE_BUTTONS::ENABLE_MOVE_Y ].IsPressed();
	bool rotZ = buttons[ STANDARD_LAYERS::PROTOTYPE_BUTTONS::ENABLE_MOVE_Z ].IsPressed();


	if( m_rotationOp.Started )
	{
		// Remember start orientation.
		CameraData& camera = globalState->Camera;

		XMVECTOR rayDir = ComputeMouseRayDirection( globalState );
		XMVECTOR rayPoint = XMVectorSetW( XMLoadFloat3( &camera.Position ), 1.0f );

		auto actorPosition = actor->GetPosition();
		auto rotVec = ComputeRotationDir( m_rotationOp.RotAxis, actorPosition, rayDir, rayPoint );

		float angle = Math::OrientedAngle( XMLoadFloat3( &m_rotationOp.StartDir ), rotVec, ComputePlaneNormal( m_rotationOp.RotAxis ) );

		if( m_rotationOp.UseStep )
		{
			float stepSize = Math::ToRadians( m_rotationOp.StepSize );
			float rotRealAngle = stepSize * roundf( angle / stepSize );
			angle = rotRealAngle;
		}

		XMVECTOR rotation = ComputeRotationQuat( m_rotationOp.RotAxis, angle );

		XMVECTOR newOrientation = XMQuaternionMultiply( XMLoadFloat4( &m_rotationOp.StartOrientation ), rotation );
		m_followedActor->TeleportOrientation( newOrientation );


		// End of rotation
		if( buttons[ STANDARD_LAYERS::PROTOTYPE_BUTTONS::LEFT_CLICK ].IsKeyUpEvent() ||
			buttons[ STANDARD_LAYERS::PROTOTYPE_BUTTONS::ENABLE_MOVE_X ].IsKeyUpEvent() ||
			buttons[ STANDARD_LAYERS::PROTOTYPE_BUTTONS::ENABLE_MOVE_Y ].IsKeyUpEvent() ||
			buttons[ STANDARD_LAYERS::PROTOTYPE_BUTTONS::ENABLE_MOVE_Z ].IsKeyUpEvent() )
		{
			m_rotationOp.Started = false;
		}
	}
	else if( buttons[ STANDARD_LAYERS::PROTOTYPE_BUTTONS::LEFT_CLICK ] && ( rotX || rotY || rotZ ) )
	{
		m_rotationOp.Started = true;
		
		if( rotX )
			m_rotationOp.RotAxis = RotationAxis::X;
		else if( rotY )
			m_rotationOp.RotAxis = RotationAxis::Y;
		else if( rotZ )
			m_rotationOp.RotAxis = RotationAxis::Z;

		CameraData& camera = globalState->Camera;

		XMVECTOR rayDir = ComputeMouseRayDirection( globalState );
		XMVECTOR rayPoint = XMVectorSetW( XMLoadFloat3( &camera.Position ), 1.0f );

		auto actorPosition = actor->GetPosition();
		auto rotVec = ComputeRotationDir( m_rotationOp.RotAxis, actorPosition, rayDir, rayPoint );

		// Remember start orientation.
		XMStoreFloat3( &m_rotationOp.StartDir, rotVec );
		XMStoreFloat4( &m_rotationOp.StartOrientation, m_followedActor->GetOrientation() );
	}

}

// ================================ //
//
void				GizmoController::Translation	( DynamicActor* actor, IControllersState* globalState )
{
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
			XMVECTOR intersection = Intersections::PlaneXZIntersection( rayDir, rayPoint, XMVectorGetY( actorPosition ) );

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

// ================================ //
//
void				GizmoController::Scaling		( DynamicActor* actor, IControllersState* globalState )
{ }



/**@brief Wylicza wektor kierunku promienia wyznaczanego przez klikniêcie mysz¹.

Wektor jest we wspó³rzêdnych lokalnych kamery, tzn przy za³o¿eniu, ¿e kamera jest skierowana w kierunku
(0.0, 0.0, -1.0). Wymaga on przemno¿enia przez kwaternion orientacji kamery.
Wektor nie jest znormalizowany.*/
DirectX::XMVECTOR	GizmoController::ComputeMouseRayDirection( IControllersState* globalState )
{
	float mouseX = (float)m_abstractionLayer->GetMouseX();
	float mouseY = (float)m_abstractionLayer->GetMouseY();

	return Intersections::ComputeMouseRayDirection( globalState->Engine, globalState->Camera, mouseX, mouseY );
}

/**@brief Computes direction of vector from actor being rotated to mouse point.*/
DirectX::XMVECTOR	GizmoController::ComputeRotationDir( RotationAxis axis, DirectX::XMVECTOR actorPos, DirectX::XMVECTOR rayDir, DirectX::XMVECTOR rayPoint )
{
	XMVECTOR planeNormal = ComputePlaneNormal( axis );

	XMVECTOR plane = XMPlaneFromPointNormal( actorPos, planeNormal );
	XMVECTOR intersection = XMPlaneIntersectLine( plane, rayPoint, XMVectorAdd( rayPoint, rayDir ) );

	return XMVectorAdd( intersection, XMVectorNegate( actorPos ) );
}

// ================================ //
//
DirectX::XMVECTOR	GizmoController::ComputeRotationQuat	( RotationAxis axis, float angle )
{
	XMVECTOR planeNormal = ComputePlaneNormal( axis );
	XMVECTOR rotation = XMQuaternionRotationNormal( planeNormal, angle );

	return rotation;
}

// ================================ //
//
DirectX::XMVECTOR	GizmoController::ComputePlaneNormal( RotationAxis axis )
{
	switch( axis )
	{
		case RotationAxis::X:
		return XMVectorSet( 1.0, 0.0, 0.0, 0.0 );
		
		case RotationAxis::Y:
		return XMVectorSet( 0.0, 1.0, 0.0, 0.0 );

		case RotationAxis::Z:
		return XMVectorSet( 0.0, 0.0, 1.0, 0.0 );

		default:
		assert( false );
	}

	return XMVectorSet( 1.0, 0.0, 0.0, 0.0 );
}


