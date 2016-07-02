#pragma once

#include "EngineEditor/EditorPlugin/Actors/ActorWrapper.h"

class IDeserializer;

namespace EditorPlugin
{
using namespace System::Collections::Generic;
using namespace System::Collections::ObjectModel;



public ref class SceneLoader
{
private:
	//ObservableCollection< ActorWrapper^ >^		m_actors;

public:

	bool			LoadScene			( System::String^ fileName );

};


}	//	EditorPlugin
