#pragma once

#include "EngineEditor/EditorPlugin/Actors/ActorWrapper.h"

class ISerializer;

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

	void			SaveScene			( System::String^ fileName );

private:
	void			SerializeActors		( ISerializer* ser );
};


}	//	EditorPlugin
