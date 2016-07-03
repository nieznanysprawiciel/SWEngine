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



/**@brief Domy�lna deserializacja w silniku.

Funkcja r�ni si� od funkcji Serialization::DefaultDeserialize.
Najpierw wywo�ywana jest wy�ej wspomniana funkcja w celu sprawdzenia typ�w podstawowych.
Potem podejmuje si� pr�b� zdeserializowania obiekt�w dziedzicz�cych po EngineObject.*/
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


/**@brief Deserializuje obiekty dziedzicz�ce po EngineObject.

Funkcja sprawdza typ w�asciwo�ci i serializuje go tylko je�eli jest jednym z obs�ugiwanych
przez ni� typ�w. W przeciwnym razie nie robi nic.

@todo Przenie�� implementacje do osobnych funkcji a tutaj zostawi� tylko wywo�ania.

@return Funkcja zwraca true, je�eli uda�o jej si� obs�u�y� podany typ.*/
bool SerializationApi::DeserializeSingleGeneric( IDeserializer* deser, rttr::property prop, const EngineObject* object )
{
	auto propertyType = prop.get_type();
	assert( propertyType.is_derived_from< EngineObject >() );

	if( propertyType == rttr::type::get< Model3DFromFile* >() )
	{
		// Przenie�� do innej funkcji. Pewnie trzeba wydzieli� z Model3DFromFile jaki� deskryptor, kt�ry by si� deserializowa�.
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

/**@brief Deserializuje struktur� ActorInfo.*/
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
