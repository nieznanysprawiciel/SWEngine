#include "EngineCore/stdafx.h"
#include "Serialization.h"

#include <codecvt>



/**@brief Zwraca wektor properties�w dla danego typu.

Propertisy s� pobierane z contextu, je�eli ju� tam istniej�. Je�eli nie to s� rejestrowane.
W ten spos�b unikamy filtrowania w�a�ciwo�ci przy ka�dym obiekcie.*/
std::vector< rttr::property >&	Serialization::GetTypeFilteredProperties( rttr::type objType, EngineSerializationContext* context )
{
	auto iter = context->TypeProperties.find( objType );
	if( iter == context->TypeProperties.end() )
	{
		auto result = context->TypeProperties.insert( std::make_pair( objType, std::vector< rttr::property >() ) );
		auto& propVec = result.first->second;

		MetaDataType saveFlag;
		if( context->SaveWholeMap )
			saveFlag = MetaDataType::Serialize;
		else
			saveFlag = MetaDataType::AllowInSaveFile;


		auto properties = objType.get_properties();
	
		for( auto& property : properties )
		{
			if( ShouldSave( property, saveFlag ) )
				propVec.push_back( property );
		}

		return propVec;
	}
	else
		return iter->second;
}

/**@brief Sprawdza czy w�a�ciwo�� powinna zosta� zapisana.

Domy�lnie:
Je�eli nie zarejestrowano metadanych, to domy�lnie w�a�ciwo�� jest serializowana.*/
bool Serialization::ShouldSave( rttr::property& prop, MetaDataType saveFlag )
{
	auto saveFlagMeta = prop.get_metadata( saveFlag );
	if( !saveFlagMeta.is_valid() )
		return true;

	assert( saveFlagMeta.is_type< bool >() );

	return saveFlagMeta.get_value< bool >();
}

/**@brief Domy�lny tryb serialziacji obiekt�w.

Serializowane s� wszystkie w�a�ciwo�ci, kt�re nie zosta�y w metadanych oznaczone
jako nieserializowalne.*/
void	Serialization::DefaultSerialize( ISerializer* ser, const EngineObject* object )
{
	auto objectType = object->GetType();
	auto& properties = GetTypeFilteredProperties( objectType, ser->GetContext< EngineSerializationContext >() );

	ser->EnterObject( objectType.get_name() );

	for( auto& property : properties )
	{
		auto propertyType = property.get_type();
		
		if( propertyType == rttr::type::get< float >() )
			SerializeProperty< float >( ser, property, object );
		else if( propertyType == rttr::type::get< double >() )
			SerializeProperty< double >( ser, property, object );
		else if( propertyType == rttr::type::get< int >() )
			SerializeProperty< double >( ser, property, object );
		else if( propertyType == rttr::type::get< DirectX::XMFLOAT2* >() )
			SerializeProperty< DirectX::XMFLOAT2* >( ser, property, object );
		else if( propertyType == rttr::type::get< DirectX::XMFLOAT3* >() )
			SerializeProperty< DirectX::XMFLOAT3* >( ser, property, object );
		else if( propertyType == rttr::type::get< DirectX::XMFLOAT4* >() )
			SerializeProperty< DirectX::XMFLOAT4* >( ser, property, object );
		else if( propertyType == rttr::type::get< std::string >() )
			SerializeProperty< std::string >( ser, property, object );
		else if( propertyType == rttr::type::get< std::wstring >() )
			SerializeProperty< std::wstring >( ser, property, object );
		else if( propertyType.is_derived_from< EngineObject >() )
			SerializeProperty< EngineObject* >( ser, property, object );
	}

	ser->Exit();	// objectType.get_name()
}

/**@brief Domy�lny tryb deserialziacji obiekt�w.

Deserializowane s� wszystkie typy podstawowe.*/
void	Serialization::DefaultDeserialize	( IDeserializer* deser, EngineObject* object )
{
	auto objectType = object->GetType();
	auto& properties = Serialization::GetTypeFilteredProperties( objectType, deser->GetContext< EngineSerializationContext >() );

	for( auto& property : properties )
	{
		auto propertyType = property.get_type();
		
		if( propertyType == rttr::type::get< float >() )
			Serialization::DeserializeProperty< float >( deser, property, object );
		else if( propertyType == rttr::type::get< double >() )
			Serialization::DeserializeProperty< double >( deser, property, object );
		else if( propertyType == rttr::type::get< int >() )
			Serialization::DeserializeProperty< double >( deser, property, object );
		else if( propertyType == rttr::type::get< DirectX::XMFLOAT2* >() )
			Serialization::DeserializeProperty< DirectX::XMFLOAT2* >( deser, property, object );
		else if( propertyType == rttr::type::get< DirectX::XMFLOAT3* >() )
			Serialization::DeserializeProperty< DirectX::XMFLOAT3* >( deser, property, object );
		else if( propertyType == rttr::type::get< DirectX::XMFLOAT4* >() )
			Serialization::DeserializeProperty< DirectX::XMFLOAT4* >( deser, property, object );
		else if( propertyType == rttr::type::get< std::string >() )
			Serialization::DeserializeProperty< std::string >( deser, property, object );
		else if( propertyType == rttr::type::get< std::wstring >() )
			Serialization::DeserializeProperty< std::wstring >( deser, property, object );
		//else if( propertyType.is_derived_from< EngineObject >() )
		//	SerializeProperty< EngineObject* >( ser, property, this );
	}
}


std::string Serialization::WstringToUTF( const std::wstring & str )
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
    return myconv.to_bytes(str);
}

std::wstring Serialization::UTFToWstring( const std::string & str )
{
	std::wstring_convert< std::codecvt_utf8< wchar_t > > myconv;
	return myconv.from_bytes( str );
}

//====================================================================================//
//				SerializeProperty template specialization
//====================================================================================//

template	void	Serialization::SerializeProperty< unsigned int >	( ISerializer* ser, rttr::property prop, const EngineObject* object );
template	void	Serialization::SerializeProperty< float >	( ISerializer* ser, rttr::property prop, const EngineObject* object );
template	void	Serialization::SerializeProperty< double >	( ISerializer* ser, rttr::property prop, const EngineObject* object );
template	void	Serialization::SerializeProperty< int >		( ISerializer* ser, rttr::property prop, const EngineObject* object );
template	void	Serialization::SerializeProperty< int16 >	( ISerializer* ser, rttr::property prop, const EngineObject* object );
template	void	Serialization::SerializeProperty< uint16 >	( ISerializer* ser, rttr::property prop, const EngineObject* object );
template	void	Serialization::SerializeProperty< int32 >	( ISerializer* ser, rttr::property prop, const EngineObject* object );
template	void	Serialization::SerializeProperty< uint32 >	( ISerializer* ser, rttr::property prop, const EngineObject* object );
template	void	Serialization::SerializeProperty< int64 >	( ISerializer* ser, rttr::property prop, const EngineObject* object );
template	void	Serialization::SerializeProperty< uint64 >	( ISerializer* ser, rttr::property prop, const EngineObject* object );

/**@brief Specjalizacja dla EngineObject.*/
template<>
void			Serialization::SerializeProperty< EngineObject* >	( ISerializer* ser, rttr::property prop, const EngineObject* object )
{
	EngineObject* engineObj = GetPropertyValue< EngineObject* >( prop, object );
	if( engineObj )
		engineObj->Serialize( ser );
}

/**@brief Specjalizacja dla DirectX::XMFLOAT3.*/
template<>
void			Serialization::SerializeProperty< DirectX::XMFLOAT3* >	( ISerializer* ser, rttr::property prop, const EngineObject* object )
{
	DirectX::XMFLOAT3* xmFloat = GetPropertyValue< DirectX::XMFLOAT3* >( prop, object );
	ser->EnterObject( prop.get_name() );

	ser->SetAttribute( "X", xmFloat->x );
	ser->SetAttribute( "Y", xmFloat->y );
	ser->SetAttribute( "Z", xmFloat->z );

	ser->Exit();	// prop.get_name()
}

/**@brief Specjalizacja dla DirectX::XMFLOAT2.*/
template<>
void			Serialization::SerializeProperty< DirectX::XMFLOAT2* >	( ISerializer* ser, rttr::property prop, const EngineObject* object )
{
	DirectX::XMFLOAT2* xmFloat = GetPropertyValue< DirectX::XMFLOAT2* >( prop, object );
	ser->EnterObject( prop.get_name() );

	ser->SetAttribute( "X", xmFloat->x );
	ser->SetAttribute( "Y", xmFloat->y );

	ser->Exit();	// prop.get_name()
}

/**@brief Specjalizacja dla DirectX::XMFLOAT4.*/
template<>
void			Serialization::SerializeProperty< DirectX::XMFLOAT4* >	( ISerializer* ser, rttr::property prop, const EngineObject* object )
{
	DirectX::XMFLOAT4* xmFloat = GetPropertyValue< DirectX::XMFLOAT4* >( prop, object );
	ser->EnterObject( prop.get_name() );

	ser->SetAttribute( "X", xmFloat->x );
	ser->SetAttribute( "Y", xmFloat->y );
	ser->SetAttribute( "Z", xmFloat->z );
	ser->SetAttribute( "W", xmFloat->w );

	ser->Exit();	// prop.get_name()
}

/**@brief Specjalizacja dla std::wstring.*/
template<>
void			Serialization::SerializeProperty< std::wstring >	( ISerializer* ser, rttr::property prop, const EngineObject* object )
{
	std::wstring str = GetPropertyValue< std::wstring >( prop, object );
	ser->SetAttribute( prop.get_name(), WstringToUTF( str ) );
}

//====================================================================================//
//				DeserializeProperty template specialization
//====================================================================================//

template	void	Serialization::DeserializeProperty< unsigned int >	( IDeserializer* deser, rttr::property prop, const EngineObject* object );
template	void	Serialization::DeserializeProperty< float >			( IDeserializer* deser, rttr::property prop, const EngineObject* object );
template	void	Serialization::DeserializeProperty< double >		( IDeserializer* deser, rttr::property prop, const EngineObject* object );
template	void	Serialization::DeserializeProperty< int >			( IDeserializer* deser, rttr::property prop, const EngineObject* object );
template	void	Serialization::DeserializeProperty< int16 >			( IDeserializer* deser, rttr::property prop, const EngineObject* object );
template	void	Serialization::DeserializeProperty< uint16 >		( IDeserializer* deser, rttr::property prop, const EngineObject* object );
template	void	Serialization::DeserializeProperty< int32 >			( IDeserializer* deser, rttr::property prop, const EngineObject* object );
template	void	Serialization::DeserializeProperty< uint32 >		( IDeserializer* deser, rttr::property prop, const EngineObject* object );
template	void	Serialization::DeserializeProperty< int64 >			( IDeserializer* deser, rttr::property prop, const EngineObject* object );
template	void	Serialization::DeserializeProperty< uint64 >		( IDeserializer* deser, rttr::property prop, const EngineObject* object );


/**@brief Specjalizacja dla DirectX::XMFLOAT3.*/
template<>
void			Serialization::DeserializeProperty< DirectX::XMFLOAT3* >	( IDeserializer* deser, rttr::property prop, const EngineObject* object )
{
	deser->EnterObject( prop.get_name() );

	DirectX::XMFLOAT3 value;
	value.x = static_cast< float > ( deser->GetAttribute( "X", TypeDefaultValue< float >() ) );
	value.y = static_cast< float > ( deser->GetAttribute( "Y", TypeDefaultValue< float >() ) );
	value.z = static_cast< float > ( deser->GetAttribute( "Z", TypeDefaultValue< float >() ) );

	SetPropertyValue( prop, object, &value );

	deser->Exit();	// prop.get_name()
}

/**@brief Specjalizacja dla DirectX::XMFLOAT2.*/
template<>
void			Serialization::DeserializeProperty< DirectX::XMFLOAT2* >	( IDeserializer* deser, rttr::property prop, const EngineObject* object )
{
	deser->EnterObject( prop.get_name() );

	DirectX::XMFLOAT2 value;
	value.x = static_cast< float > ( deser->GetAttribute( "X", TypeDefaultValue< float >() ) );
	value.y = static_cast< float > ( deser->GetAttribute( "Y", TypeDefaultValue< float >() ) );

	SetPropertyValue( prop, object, &value );

	deser->Exit();	// prop.get_name()
}

/**@brief Specjalizacja dla DirectX::XMFLOAT4.*/
template<>
void			Serialization::DeserializeProperty< DirectX::XMFLOAT4* >	( IDeserializer* deser, rttr::property prop, const EngineObject* object )
{
	deser->EnterObject( prop.get_name() );

	DirectX::XMFLOAT4 value;
	value.x = static_cast< float > ( deser->GetAttribute( "X", TypeDefaultValue< float >() ) );
	value.y = static_cast< float > ( deser->GetAttribute( "Y", TypeDefaultValue< float >() ) );
	value.z = static_cast< float > ( deser->GetAttribute( "Z", TypeDefaultValue< float >() ) );
	value.w = static_cast< float > ( deser->GetAttribute( "W", TypeDefaultValue< float >() ) );

	SetPropertyValue( prop, object, &value );

	deser->Exit();	// prop.get_name()
}

/**@brief Specjalizacja dla std::wstring.*/
template<>
void			Serialization::DeserializeProperty< std::wstring >	( IDeserializer* deser, rttr::property prop, const EngineObject* object )
{
	std::wstring str = GetPropertyValue< std::wstring >( prop, object );
	SetPropertyValue( prop, object, UTFToWstring( deser->GetAttribute( prop.get_name(), TypeDefaultValue< std::string >() ) ) );
}
