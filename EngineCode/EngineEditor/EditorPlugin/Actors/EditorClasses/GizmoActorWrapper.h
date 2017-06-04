#pragma once
/**
@file GizmoActorWrapper.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/



#include "EngineEditor/EditorPlugin/Actors/ActorWrapper.h"
#include "EngineCore/ControllersEngine/BasicControllers/Editor/GizmoController.h"


namespace sw {
namespace EditorPlugin
{

public enum class TransformType
{
	Translation,
	Rotation,
	Scale
};


/**@brief */
public ref class GizmoActorWrapper : public ActorWrapper
{
private:
public:
	GizmoActorWrapper( EngineObject* actor, const ActorInfo* actorInfo );


	// ================================ //
	// Properties


	property TransformType	TransformMode
	{
		TransformType	get();
		void			set( TransformType value );
	}


	property bool		UseRotationStep
	{
		bool	get();
		void	set( bool value );
	}

	property float		RotationStepSize
	{
		float	get();
		void	set( float value );
	}


protected:
	TransformType					Translate	( GizmoController::Operation op );
	GizmoController::Operation		Translate	( TransformType op );

};


}	// EditorPlugin
}	// sw
