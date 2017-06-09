#include "GizmoActorWrapper.h"



#include "EngineCore/ControllersEngine/BasicControllers/Editor/GizmoController.h"




namespace sw {
namespace EditorPlugin
{

using namespace sw::editor;


/**@brief Helper for getting typed GizmoController from actor.*/
GizmoController*		GetController	( ActorBase* actor )
{
	auto gizmoPtr = static_cast<DynamicActor*>( actor );
	assert( rttr::rttr_cast<DynamicActor*>( gizmoPtr ) );

	auto gizmoController = static_cast<GizmoController*>( gizmoPtr->GetController() );
	assert( rttr::rttr_cast<GizmoController*>( gizmoController ) );

	return gizmoController;
}


GizmoActorWrapper::GizmoActorWrapper( EngineObject* actor, const ActorInfo* actorInfo )
	: ActorWrapper( actor, actorInfo )
{}

// ================================ //
//
TransformType				GizmoActorWrapper::Translate( GizmoController::Operation op )
{
	switch( op )
	{
		case GizmoController::Operation::Translate:
			return TransformType::Translation;
			break;
		case GizmoController::Operation::Rotate:
			return TransformType::Rotation;
			break;
		case GizmoController::Operation::Scale:
			return TransformType::Scale;
			break;
		default:
			return TransformType::Translation;
			break;
	}
}

// ================================ //
//
GizmoController::Operation	GizmoActorWrapper::Translate( TransformType op )
{
	switch( op )
	{
		case EditorPlugin::TransformType::Translation:
			return GizmoController::Operation::Translate;
			break;
		case EditorPlugin::TransformType::Rotation:
			return GizmoController::Operation::Rotate;
			break;
		case EditorPlugin::TransformType::Scale:
			return GizmoController::Operation::Scale;
			break;
		default:
			return GizmoController::Operation::Translate;
			break;
	}
}


// ================================ //
//
void				GizmoActorWrapper::TransformMode::set( TransformType value )
{
	auto gizmoCtrl = GetController( Ptr() );
	gizmoCtrl->ChangeOperation( Translate( value ) );
}

// ================================ //
//
TransformType		GizmoActorWrapper::TransformMode::get()
{
	auto gizmoCtrl = GetController( Ptr() );
	return Translate( gizmoCtrl->CheckOperation() );
}



// ================================ //
//
bool		GizmoActorWrapper::UseRotationStep::get()
{
	auto gizmoCtrl = GetController( Ptr() );
	return gizmoCtrl->GetUseStepRotation();
}

// ================================ //
//
void		GizmoActorWrapper::UseRotationStep::set( bool value )
{
	auto gizmoCtrl = GetController( Ptr() );
	gizmoCtrl->UseStepRotation( value );
}

// ================================ //
//
float		GizmoActorWrapper::RotationStepSize::get()
{
	auto gizmoCtrl = GetController( Ptr() );
	return gizmoCtrl->GetStepSize();
}

// ================================ //
//
void		GizmoActorWrapper::RotationStepSize::set( float value )
{
	auto gizmoCtrl = GetController( Ptr() );
	gizmoCtrl->SetStepSize( value );
}

}
}	// sw

