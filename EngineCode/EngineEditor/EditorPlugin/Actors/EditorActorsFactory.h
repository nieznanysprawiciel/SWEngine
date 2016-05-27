#pragma once
/**
@file EditorActorsFactory.h
@author nieznanysprawiciel
@copyright Plik jest cz�ci� silnika graficznego SWEngine.
*/

#include "EngineEditor/EditorPlugin/Actors/ActorWrapper.h"



namespace EditorPlugin
{

/**@brief S�u�y do tworzenia aktor�w pomocniczych dla Edytora.*/
public ref class EditorActorsFactory
{
private:
public:
	static ActorWrapper^			CreateGizmoActor		( System::String^ meshPath );
};

}	// EditorPlugin
