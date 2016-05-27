#pragma once
/**
@file EditorActorsFactory.h
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.
*/

#include "EngineEditor/EditorPlugin/Actors/ActorWrapper.h"



namespace EditorPlugin
{

/**@brief S³u¿y do tworzenia aktorów pomocniczych dla Edytora.*/
public ref class EditorActorsFactory
{
private:
public:
	static ActorWrapper^			CreateGizmoActor		( System::String^ meshPath );
};

}	// EditorPlugin
