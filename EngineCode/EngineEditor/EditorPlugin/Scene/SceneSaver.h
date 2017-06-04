#pragma once
/**
@file SceneSaver.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/


#include "EngineEditor/EditorPlugin/Actors/ActorWrapper.h"

class ISerializer;




namespace sw {
namespace EditorPlugin
{


using namespace System::Collections::Generic;
using namespace System::Collections::ObjectModel;



public ref class SceneSaver
{
private:
	ObservableCollection< ActorWrapper^ >^		m_actors;

public:

	void			ActorsToSave		( ObservableCollection< ActorWrapper^ >^ actors );

	bool			SaveScene			( System::String^ fileName );

private:
	void			SerializeActors		( ISerializer* ser );
};


}	//	EditorPlugin
}	// sw
