#include "SceneLoader.h"

#include "EngineEditor/EditorPlugin/Native/SceneHelpers.h"
#include "EngineEditor/EditorPlugin/EnginePointerProvider.h"
#include "EngineCore/Actors/ActorsManager.h"



#include <msclr/marshal_cppstd.h>


namespace EditorPlugin
{


SceneLoader::SceneLoader()
	: m_actors( nullptr )
{}


/**@brief */
bool SceneLoader::LoadScene( System::String^ fileName )
{
	auto engine = EnginePointerProvider::GetEngine();
	std::string filePath = msclr::interop::marshal_as< std::string >( fileName );

	Api::LevelApi::EditorLoadResult result = engine->Level.EditorLoadLevelSync( filePath );
	if( !result.Success )
		return false;

	m_actors = CreateActorsList( result );
	return true;
}

/**@brief */
ObservableCollection<ActorWrapper^>^ SceneLoader::CreateActorsList( Api::LevelApi::EditorLoadResult& result )
{
	auto engine = EnginePointerProvider::GetEngine();

	ObservableCollection< ActorWrapper^ >^ actorsList = gcnew ObservableCollection< ActorWrapper^ >();
	auto& newActors = result.Actors;
	auto& actorsInfos = result.ActorsInfo;
	auto& actorsNames = result.ActorsNames;

	for( int i = 0; i < newActors.size(); i++ )
	{
		ActorWrapper^ newActor = gcnew ActorWrapper( (EngineObject*)newActors[ i ], &actorsInfos[ i ] );
		newActor->ActorName = gcnew System::String( actorsNames[ i ].c_str() );
		actorsList->Add( newActor );
	}

	return actorsList;
}

/**@brief Zwraca wczytanych aktorów.

Je¿eli nie wywo³ano wczeœniej funkcji LoadScene lub wywo³anie nie uda³o siê,
to zwrócony zostanie nullptr.*/
ObservableCollection<ActorWrapper^>^ SceneLoader::GetLoadedActors()
{
	return m_actors;
}



}	//	EditorPlugin