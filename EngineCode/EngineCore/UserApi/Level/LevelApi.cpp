#include "EngineCore/stdafx.h"
#include "LevelApi.h"

#include "EngineCore/MainEngine/Engine.h"
#include "EngineCore/MainEngine/EngineContext.h"

#include "Common/Serialization/Deserializer.h"
#include "Common/Serialization/SW/EngineSerializationContext.h"


namespace Api
{


const char*		LevelApi::ACTORS_ARRAY_STRING			= "Actors";
const char*		LevelApi::ACTOR_ARRAY_ELEMENT_STRING	= "Actor";
const char*		LevelApi::ACTOR_OBJECT_NAME				= "ActorName";
const char*		LevelApi::LOAD_ACTOR_NAME				= "UseName";


/**@brief Wczytuje level.*/
bool LevelApi::LoadLevelSync( const std::string& levelFileName )
{
	auto context = std::make_unique< EngineSerializationContext >();
	context->EditorLoad = false;

	IDeserializer deser( std::move( context ) );
	EditorLoadResult editorData;		// Dane nie bêd¹ wype³niane dziêki fladze context->EditorLoad.

	bool result = deser.LoadFromFile( levelFileName, ParsingMode::ParseInsitu );
	if( result )
	{
		editorData = LoadLevelImpl( &deser );
		result = editorData.Success;
	}

	return result;
}

/**@brief Wczytuje level i zwraca informacje potrzebne edytorowi.*/
LevelApi::EditorLoadResult	LevelApi::EditorLoadLevelSync	( const std::string& levelFileName )
{
	auto context = std::make_unique< EngineSerializationContext >();
	context->EditorLoad = true;

	IDeserializer deser( std::move( context ) );
	EditorLoadResult editorData;

	editorData.Success = deser.LoadFromFile( levelFileName, ParsingMode::ParseInsitu );
	if( editorData.Success )
		editorData = LoadLevelImpl( &deser );

	return editorData;
}


LevelApi::EditorLoadResult	LevelApi::LoadLevelImpl			( IDeserializer* deser )
{
	EditorLoadResult editorData;
	auto context = deser->GetContext< EngineSerializationContext >();

	if( deser->EnterArray( LevelApi::ACTORS_ARRAY_STRING ) )
	{
		if( deser->FirstElement() )
		{
			do
			{
				auto actorName = deser->GetAttribute( LevelApi::ACTOR_OBJECT_NAME, "" );
				bool useName = deser->GetAttribute( LevelApi::LOAD_ACTOR_NAME, false );

				if( deser->EnterObject( Level::SerializationApi::ACTOR_INFO_STRING ) )
				{
					ActorInfo info = Serialization.DeserializeActorInfo( deser );
					
					// Przecohdzimy do kolejnego obiektu, którm powinien byæ aktor.
					bool actorObjectExists = deser->NextElement();
					assert( actorObjectExists );

					auto newActor = Context->actorsManager->CreateActor( deser->GetName() );
					assert( newActor );

					if( context->EditorLoad )
					{
						// Dodajemy tylko do podstawowych modu³ów, za to do edytora wysy³amy pe³ne informacje.
						ActorInfo editorModeInfo( EnableDisplay | AsCamera | AsLight );
						editorModeInfo &= info;
						Engine->Actors.AddToModules( newActor, editorModeInfo );
					}
					else
						Engine->Actors.AddToModules( newActor, info );

					newActor->Deserialize( deser );

					if( useName && strcmp( actorName, "" ) )
						Context->actorsManager->NameActor( newActor, actorName );

					if( context->EditorLoad )
					{
						editorData.Actors.push_back( newActor );
						editorData.ActorsInfo.push_back( info );
						editorData.ActorsNames.push_back( actorName );
					}

					deser->Exit();	// LevelApi::ACTOR_INFO_STRING
				}

			} while( deser->NextElement() );
		}

		deser->Exit();	// ACTORS_ARRAY_STRING
	}

	editorData.Success = true;
	return editorData;
}

}	// Api
