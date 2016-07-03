#pragma once

#include "EngineEditor/EditorPlugin/Actors/ActorWrapper.h"

class IDeserializer;

namespace Api
{
	struct LevelApi::EditorLoadResult;
}

namespace EditorPlugin
{
using namespace System::Collections::Generic;
using namespace System::Collections::ObjectModel;



public ref class SceneLoader
{
private:
	ObservableCollection< ActorWrapper^ >^		m_actors;

public:
	SceneLoader();

	bool			LoadScene			( System::String^ fileName );

public:
	ObservableCollection< ActorWrapper^ >^		GetLoadedActors();


private:

	ObservableCollection< ActorWrapper^ >^		CreateActorsList( Api::LevelApi::EditorLoadResult& result );
};


}	//	EditorPlugin
