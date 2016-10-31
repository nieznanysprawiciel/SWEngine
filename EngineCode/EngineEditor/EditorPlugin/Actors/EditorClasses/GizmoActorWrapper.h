#pragma once

#include "EngineEditor/EditorPlugin/Actors/ActorWrapper.h"


namespace EditorPlugin
{



public ref class GizmoActorWrapper : public ActorWrapper
{
private:
public:
	GizmoActorWrapper( EngineObject* actor, const ActorInfo* actorInfo );


	// ================================ //
	// Properties


	property bool		TranslationMode
	{
		bool	get();
		void	set( bool value );
	}

	property bool		RotationMode
	{
		bool	get();
		void	set( bool value );
	}

	property bool		ScaleMode
	{
		bool	get();
		void	set( bool value );
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

};


}	// EditorPlugin
