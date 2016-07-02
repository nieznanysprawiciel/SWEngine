#include "SceneLoader.h"

#include "EngineEditor/EditorPlugin/Native/SceneHelpers.h"
#include "EngineEditor/EditorPlugin/EnginePointerProvider.h"
#include "EngineCore/Actors/ActorsManager.h"



#include <msclr/marshal_cppstd.h>


namespace EditorPlugin
{

/**@brief */
bool SceneLoader::LoadScene( System::String^ fileName )
{
	auto engine = EnginePointerProvider::GetEngine();
	std::string filePath = msclr::interop::marshal_as< std::string >( fileName );

	bool result = engine->Level.LoadLevelSync( filePath );

	auto& actors = engine->Actors.GetAllActors();
	for( auto& actorData : actors )
	{
		if( actorData.second.EnableDisplay() )
			engine->Actors.AddToModules( actorData.first, ActorInfoFlag::EnableDisplay );
	}


	return result;
}



}	//	EditorPlugin