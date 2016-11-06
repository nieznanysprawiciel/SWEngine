#pragma once
/**
@file EditorActorsFactory.h
@author nieznanysprawiciel
@copyright Plik jest cz�ci� silnika graficznego SWEngine.
*/

#include "EngineEditor/EditorPlugin/Actors/ActorWrapper.h"
#include "EngineEditor/EditorPlugin/Actors/EditorClasses/GizmoActorWrapper.h"



namespace EditorPlugin
{

/**@brief S�u�y do tworzenia aktor�w pomocniczych dla Edytora.*/
public ref class EditorActorsFactory
{
private:
public:
	static GizmoActorWrapper^		CreateGizmoActor		( System::String^ translateMesh, System::String^ rotateMesh, System::String^ scaleMesh );
	static ActorWrapper^			CreateDefaultCamera		( bool setAsCurrent );
};

}	// EditorPlugin
