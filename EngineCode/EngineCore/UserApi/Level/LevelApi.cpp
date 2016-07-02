#include "EngineCore/stdafx.h"
#include "LevelApi.h"

#include "EngineCore/MainEngine/EngineContext.h"

#include "Common/Serialization/Deserializer.h"
#include "Common/Serialization/SW/EngineSerializationContext.h"


namespace Api
{


/**@brief Wczytuje level.*/
bool LevelApi::LoadLevelSync( const std::string& levelFileName )
{
	auto context = std::make_unique< EngineSerializationContext >();
	IDeserializer deser( std::move( context ) );

	bool result = deser.LoadFromFile( levelFileName, ParsingMode::ParseInsitu );
	if( result )
		Context->actorsManager->Deserialize( &deser );

	return result;
}

}	// Api
