#include "EngineCore/stdafx.h"
#include "SerializationApi.h"

#include "EngineCore/MainEngine/EngineContext.h"

#include "Common/Serialization/Deserializer.h"
#include "Common/Serialization/Serializer.h"
#include "Common/Serialization/SW/EngineSerializationContext.h"
#include "Common/Serialization/SW/Serialization.h"


namespace Api { namespace Level
{

const char*		SerializationApi::ACTOR_INFO_STRING						= "ActorInfo";

const char*		SerializationApi::ACTOR_INFO_ENABLE_DISPLAY				= "EnableDisplay";
const char*		SerializationApi::ACTOR_INFO_ENABLE_MOVEMENT			= "EnableMovement";
const char*		SerializationApi::ACTOR_INFO_ENABLE_PRE_CONTROLLERS		= "EnablePreController";
const char*		SerializationApi::ACTOR_INFO_ENABLE_POST_CONTROLLERS	= "EnablePostController";
const char*		SerializationApi::ACTOR_INFO_ENABLE_PHYSIC				= "EnablePhysic";
const char*		SerializationApi::ACTOR_INFO_ENABLE_SHADOW				= "EnableShadow";
const char*		SerializationApi::ACTOR_INFO_ENABLE_COLLISION			= "EnableCollisions";
const char*		SerializationApi::ACTOR_INFO_IS_LIGHT					= "IsLight";
const char*		SerializationApi::ACTOR_INFO_IS_CAMERA					= "IsCamera";
const char*		SerializationApi::ACTOR_INFO_ENABLE_SAVING_TO_FILE		= "EnableSavingToFile";



/**@brief Domyœlna deserializacja w silniku.

Funkcja ró¿ni siê od funkcji Serialization::DefaultDeserialize.
Najpierw wywo³ywana jest wy¿ej wspomniana funkcja w celu sprawdzenia typów podstawowych.
Potem podejmuje siê próbê zdeserializowania obiektów dziedzicz¹cych po EngineObject.*/
void SerializationApi::DefaultDeserialize( IDeserializer* deser, EngineObject* object )
{
	auto objectType = object->GetType();
	auto& properties = Serialization::GetTypeFilteredProperties( objectType, deser->GetContext< EngineSerializationContext >() );

	for( auto& property : properties )
	{
		auto propertyType = property.get_type();
		
		bool deserialized = Serialization::DeserializeBasicTypes( deser, object, property ) ||
							Serialization::DeserializeVectorTypes( deser, object, property ) ||
							Serialization::DeserializeStringTypes( deser, object, property ) ||
							DeserializeSingleGeneric( deser, property, object );
	}
}


/**@brief Deserializuje obiekty dziedzicz¹ce po EngineObject.

Funkcja sprawdza typ w³asciwoœci i serializuje go tylko je¿eli jest jednym z obs³ugiwanych
przez ni¹ typów. W przeciwnym razie nie robi nic.

@todo Przenieœæ implementacje do osobnych funkcji a tutaj zostawiæ tylko wywo³ania.

@return Funkcja zwraca true, je¿eli uda³o jej siê obs³u¿yæ podany typ.*/
bool SerializationApi::DeserializeSingleGeneric( IDeserializer* deser, rttr::property prop, const EngineObject* object )
{
	auto propertyType = prop.get_type();
	assert( propertyType.is_derived_from< EngineObject >() );

	if( propertyType == rttr::type::get< Model3DFromFile* >() )
	{
		// Przenieœæ do innej funkcji. Pewnie trzeba wydzieliæ z Model3DFromFile jakiœ deskryptor, który by siê deserializowa³.
		if( deser->EnterObject( prop.get_name() ) )
		{
			if( deser->EnterObject( rttr::type::get< Model3DFromFile >().get_name() ) )
			{
				std::wstring filePath = Serialization::UTFToWstring( deser->GetAttribute( "FileName", "" ) );
		
				auto result = Context->modelsManager->LoadModelFromFile( filePath );
				if( result != MODELS_MANAGER_OK )
					return false;

				auto mesh = Context->modelsManager->GetModel( filePath );
				Serialization::SetPropertyValue< Model3DFromFile* >( prop, object, mesh );

				deser->Exit();	// Model3DFromFile
			}
			deser->Exit();	// prop.get_name()

			return true;
		}
	}

	return false;
}

void SerializationApi::SerializeActorInfo( ISerializer* ser, ActorInfo info )
{
	ser->EnterObject( SerializationApi::ACTOR_INFO_STRING );

	ser->SetAttribute( SerializationApi::ACTOR_INFO_ENABLE_DISPLAY, info.EnableDisplay() );
	ser->SetAttribute( SerializationApi::ACTOR_INFO_ENABLE_MOVEMENT, info.EnableMovement() );
	ser->SetAttribute( SerializationApi::ACTOR_INFO_ENABLE_PHYSIC, info.EnablePhysic() );
	ser->SetAttribute( SerializationApi::ACTOR_INFO_ENABLE_COLLISION, info.EnableCollisions() );
	ser->SetAttribute( SerializationApi::ACTOR_INFO_ENABLE_SHADOW, info.EnableShadow() );
	ser->SetAttribute( SerializationApi::ACTOR_INFO_ENABLE_PRE_CONTROLLERS, info.EnablePreController() );
	ser->SetAttribute( SerializationApi::ACTOR_INFO_ENABLE_POST_CONTROLLERS, info.EnablePostController() );
	ser->SetAttribute( SerializationApi::ACTOR_INFO_IS_LIGHT, info.IsLight() );
	ser->SetAttribute( SerializationApi::ACTOR_INFO_IS_CAMERA, info.IsCamera() );
	ser->SetAttribute( SerializationApi::ACTOR_INFO_ENABLE_SAVING_TO_FILE, info.EnableSavingToFile() );

	ser->Exit();	// SerializationApi::ACTOR_INFO_STRING
}

/**@brief Deserializuje strukturê ActorInfo.*/
ActorInfo SerializationApi::DeserializeActorInfo( IDeserializer* deser )
{
	ActorInfo info;
	info += deser->GetAttribute( SerializationApi::ACTOR_INFO_ENABLE_DISPLAY, false ) ? ActorInfoFlag::EnableDisplay : ActorInfoFlag::DisableAll;
	info += deser->GetAttribute( SerializationApi::ACTOR_INFO_ENABLE_MOVEMENT, false ) ? ActorInfoFlag::EnableMovement : ActorInfoFlag::DisableAll;
	info += deser->GetAttribute( SerializationApi::ACTOR_INFO_ENABLE_PHYSIC, false ) ? ActorInfoFlag::EnablePhysic : ActorInfoFlag::DisableAll;
	info += deser->GetAttribute( SerializationApi::ACTOR_INFO_ENABLE_COLLISION, false ) ? ActorInfoFlag::EnableCollisions : ActorInfoFlag::DisableAll;
	info += deser->GetAttribute( SerializationApi::ACTOR_INFO_ENABLE_SHADOW, false ) ? ActorInfoFlag::EnableShadow : ActorInfoFlag::DisableAll;
	info += deser->GetAttribute( SerializationApi::ACTOR_INFO_ENABLE_PRE_CONTROLLERS, false ) ? ActorInfoFlag::EnablePreController : ActorInfoFlag::DisableAll;
	info += deser->GetAttribute( SerializationApi::ACTOR_INFO_ENABLE_POST_CONTROLLERS, false ) ? ActorInfoFlag::EnablePostController : ActorInfoFlag::DisableAll;
	info += deser->GetAttribute( SerializationApi::ACTOR_INFO_IS_LIGHT, false ) ? ActorInfoFlag::AsLight : ActorInfoFlag::DisableAll;
	info += deser->GetAttribute( SerializationApi::ACTOR_INFO_IS_CAMERA, false ) ? ActorInfoFlag::AsCamera : ActorInfoFlag::DisableAll;
	info += deser->GetAttribute( SerializationApi::ACTOR_INFO_ENABLE_SAVING_TO_FILE, false ) ? ActorInfoFlag::EnableSavingToFile : ActorInfoFlag::DisableAll;

	return info;
}

}	// Level
}	// Api
