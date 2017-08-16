#pragma once
/**
@file SceneLoader.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/



#include "EngineEditor/EditorPlugin/Actors/ActorWrapper.h"

#include "EngineCore/UserApi/Level/LevelApi.h"

class IDeserializer;


namespace sw
{


namespace Api
{
	class LevelApi;
	struct LevelApi::EditorLoadResult;
}

namespace EditorPlugin
{


using namespace System::Collections::Generic;
using namespace System::Collections::ObjectModel;



// ================================ //
//
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
}	// sw
