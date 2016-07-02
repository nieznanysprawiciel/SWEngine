#include "EngineCore/stdafx.h"
#include "SerializationApi.h"

#include "EngineCore/MainEngine/EngineContext.h"

#include "Common/Serialization/Deserializer.h"
#include "Common/Serialization/Serializer.h"
#include "Common/Serialization/SW/EngineSerializationContext.h"
#include "Common/Serialization/SW/Serialization.h"


namespace Api
{

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

}	// Api
