#pragma warning(disable : 4561)	// DirectXMath converting fastcall to stdcall
#include "SceneSaver.h"

#include "EngineEditor/EditorPlugin/Native/SceneHelpers.h"
#include "EngineEditor/EditorPlugin/EnginePointerProvider.h"
#include "EngineCore/Actors/ActorsManager.h"

#include "swCommonLib/Serialization/Serializer.h"


#include <msclr/marshal_cppstd.h>




namespace sw {
namespace EditorPlugin
{

/**@brief */
void		SceneSaver::ActorsToSave( ObservableCollection<ActorWrapper^>^ actors )
{
	m_actors = actors;
}


/**@brief */
bool		SceneSaver::SaveScene( System::String^ fileName )
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
void		SceneSaver::SerializeActors( ISerializer* ser )
{
	auto engine = EnginePointerProvider::GetEngine();

	ser->EnterArray( Api::LevelApi::ACTORS_ARRAY_STRING );

	for each( auto obj in m_actors )
	{
		ser->EnterObject( Api::LevelApi::ACTOR_ARRAY_ELEMENT_STRING );

		ser->SetAttribute( Api::LevelApi::ACTOR_OBJECT_NAME, msclr::interop::marshal_as< std::string >( obj->ActorName ) );

		engine->Level.Serialization.SerializeActorInfo( ser, obj->GetActorInfo() );
		obj->Ptr()->Serialize( ser );

		ser->Exit();	// Api::LevelApi::ACTOR_ARRAY_ELEMENT_STRING
	}

	ser->Exit();	// Api::LevelApi::ACTORS_ARRAY_STRING
}

}	//	EditorPlugin
}	// sw


