#include "SceneSaver.h"

#include "EngineEditor/EditorPlugin/Native/SceneHelpers.h"
#include "EngineEditor/EditorPlugin/EnginePointerProvider.h"
#include "EngineCore/Actors/ActorsManager.h"

#include "Common/Serialization/Serializer.h"


#include <msclr/marshal_cppstd.h>


namespace EditorPlugin
{

const std::string		ACTORS_ARRAY_NAME		= "Actors";
const std::string		ACTOR_INSTANCE_NAME		= "Actor";


/**@brief */
void SceneSaver::ActorsToSave( ObservableCollection<ActorWrapper^>^ actors )
{
	m_actors = actors;
}


/**@brief */
bool SceneSaver::SaveScene( System::String^ fileName )
{
	auto engine = EnginePointerProvider::GetEngine();
	std::string filePath = msclr::interop::marshal_as< std::string >( fileName );

	auto ser = SceneHelpers::CreateSerializerWithContext();	
	SerializeActors( ser );

	bool result = ser->SaveFile( filePath, WritingMode::Readable );
	delete ser;

	return result;
}


/**@brief */
void SceneSaver::SerializeActors( ISerializer* ser )
{
	ser->EnterArray( ACTORS_ARRAY_NAME );

	for each( auto obj in m_actors )
	{
		ser->EnterObject( ACTOR_INSTANCE_NAME );

		ser->SetAttribute( "ActorName", msclr::interop::marshal_as< std::string >( obj->ActorName ) );

		ser->EnterObject( "ActorInfo");

		ser->SetAttribute( "EnableDisplay", obj->EnableDisplay );
		ser->SetAttribute( "EnableMovement", obj->EnableMovement );
		ser->SetAttribute( "EnablePhysic", obj->EnablePhysic );
		ser->SetAttribute( "EnableCollisions", obj->EnableCollisions );
		ser->SetAttribute( "EnableShadow", obj->EnableShadow );
		ser->SetAttribute( "EnablePreController", obj->EnablePreController );
		ser->SetAttribute( "EnablePostController", obj->EnablePostController );
		ser->SetAttribute( "IsLight", obj->IsLight );
		ser->SetAttribute( "IsCamera", obj->IsCamera );
		ser->SetAttribute( "EnableSavingToFile", obj->EnableSavingToFile );

		ser->Exit();

		obj->Ptr()->Serialize( ser );
		ser->Exit();
	}

	ser->Exit();
}

}	//	EditorPlugin