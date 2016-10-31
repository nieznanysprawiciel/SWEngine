#include "GizmoActorWrapper.h"



#include "EngineCore/ControllersEngine/BasicControllers/Editor/GizmoController.h"


namespace EditorPlugin
{

/**@brief Helper for getting typed GizmoController from actor.*/
GizmoController*		GetController	( ActorBase* actor )
{
	auto gizmoPtr = static_cast< DynamicActor* >( actor );
	assert( rttr::rttr_cast< DynamicActor* >( gizmoPtr ) );

	auto gizmoController = static_cast< GizmoController* >( gizmoPtr->GetController() );
	assert( rttr::rttr_cast< GizmoController* >( gizmoController ) );

	return gizmoController;
}


GizmoActorWrapper::GizmoActorWrapper( EngineObject* actor, const ActorInfo* actorInfo )
	:	ActorWrapper( actor, actorInfo )
{}


// ================================ //
//
void		GizmoActorWrapper::TranslationMode::set( bool value )
{
	if( value )
	{
		auto gizmoCtrl = GetController( Ptr() );
		gizmoCtrl->ChangeOperation( GizmoController::Operation::Translate );
	}
}

// ================================ //
//
void		GizmoActorWrapper::RotationMode::set( bool value )
{
	if( value )
	{
		auto gizmoCtrl = GetController( Ptr() );
		gizmoCtrl->ChangeOperation( GizmoController::Operation::Rotate );
	}
}

// ================================ //
//
void		GizmoActorWrapper::ScaleMode::set( bool value )
{
	if( value )
	{
		auto gizmoCtrl = GetController( Ptr() );
		gizmoCtrl->ChangeOperation( GizmoController::Operation::Scale );
	}
}

// ================================ //
//
bool		GizmoActorWrapper::TranslationMode::get()
{
	auto gizmoCtrl = GetController( Ptr() );
	if( gizmoCtrl->CheckOperation() == GizmoController::Operation::Translate )
		return true;
	return false;
}

// ================================ //
//
bool		GizmoActorWrapper::RotationMode::get()
{
	auto gizmoCtrl = GetController( Ptr() );
	if( gizmoCtrl->CheckOperation() == GizmoController::Operation::Rotate )
		return true;
	return false;
}

// ================================ //
//
bool		GizmoActorWrapper::ScaleMode::get()
{
	auto gizmoCtrl = GetController( Ptr() );
	if( gizmoCtrl->CheckOperation() == GizmoController::Operation::Scale )
		return true;
	return false;
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

