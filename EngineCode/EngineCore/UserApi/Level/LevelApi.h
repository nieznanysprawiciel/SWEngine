#pragma once
/**@file LevelApi.h
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.

@brief Deklaracja klasy InputApi.*/

class ActorBase;
class IDeserializer;

#include "EngineCore/UserApi/ApiGroup.h"
#include "EngineCore/UserApi/Level/SerializationApi.h"
#include "EngineCore/Actors/ActorInfo.h"

#include <string>

namespace sw {
namespace Api
{

/**@defgroup LevelApi Zarz¹dzanie levelami.
@ingroup UserAPI
@brief Funkcje do wczytywania levelu.*/

	/**@brief Zawiera funkcje do wczytywania levelu.
	@ingroup UserAPI
	@ingroup LevelApi*/
class LevelApi : public ApiGroup
{
public:
	static const char*	ACTORS_ARRAY_STRING;
	static const char*	ACTOR_ARRAY_ELEMENT_STRING;
	static const char*	ACTOR_OBJECT_NAME;
	static const char*	LOAD_ACTOR_NAME;

	struct EditorLoadResult
	{
		bool						Success;
		std::vector< ActorBase* >	Actors;
		std::vector< std::string >	ActorsNames;
		std::vector< ActorInfo >	ActorsInfo;
	};

private:
protected:
public:
	LevelApi() = default;
	~LevelApi() = default;
public:

	Level::SerializationApi		Serialization;	///< Funkcje pomocnicze do serializacji.

public:

	bool				LoadLevelSync		( const std::string& levelFileName );
	EditorLoadResult	EditorLoadLevelSync	( const std::string& levelFileName );

private:

	EditorLoadResult	LoadLevelImpl		( IDeserializer* deser );
};



}	// Api
}	// sw
