#include "PropertyWrapper.h"
#include <msclr/marshal_cppstd.h>

#include "ResourceObjectPropertyWrapper.h"

namespace EditorPlugin
{





//====================================================================================//
//			PropertyWrapper implementation	
//====================================================================================//


/**@brief */
void			PropertyWrapper::ResetActor		( System::IntPtr objectPtr )
{
	m_actorPtr = static_cast< EngineObject* >( objectPtr.ToPointer() );
}


#pragma region UnusedXMFloatProperties
/**************************************************************************************/
//====================================================================================//
//				DirectXMath types	
//====================================================================================//


//====================================================================================//
//				Float2PropertyWrapper
//====================================================================================//



/**@brief */
float		Float2PropertyWrapper::GetValueX	( EngineObject* refObject )
{
	auto value = GetPropertyValue< DirectX::XMFLOAT2 >( m_metaProperty, System::IntPtr( refObject ) );
	return value.x;
}

/**@brief */
void		Float2PropertyWrapper::SetValueX	( EngineObject* refObject, float newValue )
{
	auto value = GetPropertyValue< DirectX::XMFLOAT2 >( m_metaProperty, System::IntPtr( refObject ) );
	value.x = newValue;
	SetPropertyValue( m_metaProperty, System::IntPtr( refObject ), value );
}

/**@brief */
float		Float2PropertyWrapper::GetValueY	( EngineObject* refObject )
{
	auto value = GetPropertyValue< DirectX::XMFLOAT2 >( m_metaProperty, System::IntPtr( refObject ) );
	return value.y;
}

/**@brief */
void		Float2PropertyWrapper::SetValueY	( EngineObject* refObject, float newValue )
{
	auto value = GetPropertyValue< DirectX::XMFLOAT2 >( m_metaProperty, System::IntPtr( refObject ) );
	value.y = newValue;
	SetPropertyValue( m_metaProperty, System::IntPtr( refObject ), value );
}

//====================================================================================//
//				Float3PropertyWrapper
//====================================================================================//
//


/**@brief */
float		Float3PropertyWrapper::GetValueX	( EngineObject* refObject )
{
	auto value = GetPropertyValue< DirectX::XMFLOAT3 >( m_metaProperty, System::IntPtr( refObject ) );
	return value.x;
}

/**@brief */
void		Float3PropertyWrapper::SetValueX	( EngineObject* refObject, float newValue )
{
	auto value = GetPropertyValue< DirectX::XMFLOAT3 >( m_metaProperty, System::IntPtr( refObject ) );
	value.x = newValue;
	SetPropertyValue( m_metaProperty, System::IntPtr( refObject ), value );
}

/**@brief */
float		Float3PropertyWrapper::GetValueY	( EngineObject* refObject )
{
	auto value = GetPropertyValue< DirectX::XMFLOAT3 >( m_metaProperty, System::IntPtr( refObject ) );
	return value.y;
}

/**@brief */
void		Float3PropertyWrapper::SetValueY	( EngineObject* refObject, float newValue )
{
	auto value = GetPropertyValue< DirectX::XMFLOAT3 >( m_metaProperty, System::IntPtr( refObject ) );
	value.y = newValue;
	SetPropertyValue( m_metaProperty, System::IntPtr( refObject ), value );
}

/**@brief */
float		Float3PropertyWrapper::GetValueZ	( EngineObject* refObject )
{
	auto value = GetPropertyValue< DirectX::XMFLOAT3 >( m_metaProperty, System::IntPtr( refObject ) );
	return value.z;
}

/**@brief */
void		Float3PropertyWrapper::SetValueZ	( EngineObject* refObject, float newValue )
{
	auto value = GetPropertyValue< DirectX::XMFLOAT3 >( m_metaProperty, System::IntPtr( refObject ) );
	value.z = newValue;
	SetPropertyValue( m_metaProperty, System::IntPtr( refObject ), value );
}


//====================================================================================//
//				Float4PropertyWrapper	
//====================================================================================//

/**@brief */
float		Float4PropertyWrapper::GetValueX	( EngineObject* refObject )
{
	auto value = GetPropertyValue< DirectX::XMFLOAT4 >( m_metaProperty, System::IntPtr( refObject ) );
	return value.x;
}

/**@brief */
void		Float4PropertyWrapper::SetValueX	( EngineObject* refObject, float newValue )
{
	auto value = GetPropertyValue< DirectX::XMFLOAT4 >( m_metaProperty, System::IntPtr( refObject ) );
	value.x = newValue;
	SetPropertyValue( m_metaProperty, System::IntPtr( refObject ), value );
}

/**@brief */
float		Float4PropertyWrapper::GetValueY	( EngineObject* refObject )
{
	auto value = GetPropertyValue< DirectX::XMFLOAT4 >( m_metaProperty, System::IntPtr( refObject ) );
	return value.y;
}

/**@brief */
void		Float4PropertyWrapper::SetValueY	( EngineObject* refObject, float newValue )
{
	auto value = GetPropertyValue< DirectX::XMFLOAT4 >( m_metaProperty, System::IntPtr( refObject ) );
	value.y = newValue;
	SetPropertyValue( m_metaProperty, System::IntPtr( refObject ), value );
}

/**@brief */
float		Float4PropertyWrapper::GetValueZ	( EngineObject* refObject )
{
	auto value = GetPropertyValue< DirectX::XMFLOAT4 >( m_metaProperty, System::IntPtr( refObject ) );
	return value.z;
}

/**@brief */
void		Float4PropertyWrapper::SetValueZ	( EngineObject* refObject, float newValue )
{
	auto value = GetPropertyValue< DirectX::XMFLOAT4 >( m_metaProperty, System::IntPtr( refObject ) );
	value.z = newValue;
	SetPropertyValue( m_metaProperty, System::IntPtr( refObject ), value );
}

/**@brief */
float		Float4PropertyWrapper::GetValueW	( EngineObject* refObject )
{
	auto value = GetPropertyValue< DirectX::XMFLOAT4 >( m_metaProperty, System::IntPtr( refObject ) );
	return value.w;
}

/**@brief */
void		Float4PropertyWrapper::SetValueW	( EngineObject* refObject, float newValue )
{
	auto value = GetPropertyValue< DirectX::XMFLOAT4 >( m_metaProperty, System::IntPtr( refObject ) );
	value.w = newValue;
	SetPropertyValue( m_metaProperty, System::IntPtr( refObject ), value );
}

#pragma endregion

//====================================================================================//
//			Regular Properties	
//====================================================================================//


/**@brief */
int IntPropertyWrapper::GetValue( EngineObject* refObject )
{
	auto value = GetPropertyValue< int >( m_metaProperty, System::IntPtr( refObject ) );
	return value;
}

/**@brief */
void IntPropertyWrapper::SetValue( EngineObject* refObject, int newValue )
{
	SetPropertyValue< int >( m_metaProperty, System::IntPtr( refObject ), newValue );
}


/**@brief */
bool BoolPropertyWrapper::GetValue( EngineObject* refObject )
{
	auto value = GetPropertyValue< bool >( m_metaProperty, System::IntPtr( refObject ) );
	return value;
}

/**@brief */
void BoolPropertyWrapper::SetValue( EngineObject* refObject, bool newValue )
{
	SetPropertyValue< bool >( m_metaProperty, System::IntPtr( refObject ), newValue );
}


/**@brief */
float FloatPropertyWrapper::GetValue( EngineObject* refObject )
{
	auto value = GetPropertyValue< float >( m_metaProperty, System::IntPtr( refObject ) );
	return value;
}

/**@brief */
void FloatPropertyWrapper::SetValue( EngineObject* refObject, float newValue )
{
	SetPropertyValue< float >( m_metaProperty, System::IntPtr( refObject ), newValue );
}

/**@brief */
double DoublePropertyWrapper::GetValue( EngineObject* refObject )
{
	auto value = GetPropertyValue< double >( m_metaProperty, System::IntPtr( refObject ) );
	return value;
}

/**@brief */
void DoublePropertyWrapper::SetValue( EngineObject* refObject, double newValue )
{
	SetPropertyValue< double >( m_metaProperty, System::IntPtr( refObject ), newValue );
}

/**@brief */
System::String ^ StringPropertyWrapper::GetValue( EngineObject* refObject )
{
	auto value = GetPropertyValue< std::string >( m_metaProperty, System::IntPtr( refObject ) );
	return gcnew System::String( value.c_str() );
}

/**@brief */
void StringPropertyWrapper::SetValue( EngineObject* refObject, System::String^ newValue )
{
	SetPropertyValue< std::string >( m_metaProperty, System::IntPtr( refObject ), msclr::interop::marshal_as< std::string >( newValue ) );
}

/**@brief */
System::String ^ WStringPropertyWrapper::GetValue( EngineObject* refObject )
{
	auto value = GetPropertyValue< std::wstring >( m_metaProperty, System::IntPtr( refObject ) );
	return gcnew System::String( value.c_str() );
}

/**@brief */
void WStringPropertyWrapper::SetValue( EngineObject* refObject, System::String^ newValue )
{
	SetPropertyValue< std::wstring >( m_metaProperty, System::IntPtr( refObject ), msclr::interop::marshal_as< std::wstring >( newValue ) );
}


//====================================================================================//
//			ObjectPropertyWrapper	CategoryPropertyWrapper
//====================================================================================//

/**@brief */
void			CategoryPropertyWrapper::ResetActor		( System::IntPtr objectPtr )
{
	m_actorPtr = static_cast< EngineObject* >( objectPtr.ToPointer() );

	for each( auto property in m_properties )
		property->ResetActor( objectPtr );
}

/**@brief */
void CategoryPropertyWrapper::BuildHierarchy( rttr::type classType )
{
	classType = classType.get_raw_type();
	auto properties = classType.get_properties();
	Dictionary< System::String^, CategoryPropertyWrapper^ >  categories;

	for( auto& prop : properties )
	{
		auto categoryNameMeta = prop.get_metadata( MetaDataType::Category );
		if( categoryNameMeta.is_valid() && categoryNameMeta.is_type< std::string >() )
		{
			const std::string& categoryNameStdStr = categoryNameMeta.get_value< std::string >();
			System::String^ categoryNameStr = gcnew System::String( categoryNameStdStr.c_str() );
			if( !categories.ContainsKey( categoryNameStr ) )
				categories[ categoryNameStr ] = gcnew CategoryPropertyWrapper( categoryNameStdStr.c_str() );

			categories[ categoryNameStr ]->Properties->Add( BuildProperty( prop ) );
		}
		else
		{
			if( !categories.ContainsKey( "Other" ) )
				categories[ "Other" ] = gcnew CategoryPropertyWrapper( "Other" );

			categories[ "Other" ]->Properties->Add( BuildProperty( prop ) );
		}	
	}

	for each ( auto var in categories )
		m_properties->Add( var.Value );
}

/**@brief */
PropertyWrapper^ CategoryPropertyWrapper::BuildProperty( rttr::property property )
{
	auto propertyType = property.get_type();
	if( propertyType == rttr::type::get< int >() )
	{
		return gcnew IntPropertyWrapper( property );
	}
	else if( propertyType == rttr::type::get< float >() )
	{
		return gcnew FloatPropertyWrapper( property );
	}
	else if( propertyType == rttr::type::get< bool >() )
	{
		return gcnew BoolPropertyWrapper( property );
	}
	else if( propertyType == rttr::type::get< double >() )
	{
		return gcnew DoublePropertyWrapper( property );
	}
	else if( propertyType == rttr::type::get< std::string >() )
	{
		return gcnew StringPropertyWrapper( property );
	}
	else if( propertyType == rttr::type::get< std::wstring >() )
	{
		return gcnew WStringPropertyWrapper( property );
	}
	else if( propertyType == rttr::type::get< DirectX::XMFLOAT2* >()
			 || propertyType == rttr::type::get< DirectX::XMFLOAT3* >() 
			 || propertyType == rttr::type::get< DirectX::XMFLOAT4* >() )
	{
		auto propertyWrapper = gcnew XMFloatPropertyWrapper( property );
		propertyWrapper->BuildHierarchy();
		return propertyWrapper;
	}
	else if( propertyType.is_derived_from< ResourceObject >() )
	{
		// Najpierw sprawdzamy czy dziedziczy po ResourceObject dopiero potem
		// EngineObject. Dziêki temu mo¿emy oddzieliæ zasoby od innych w³aœciowoœci
		// i obs³u¿yæ je inaczej.
		auto propertyWrapper = gcnew ResourceObjectPropertyWrapper( property );
		propertyWrapper->BuildHierarchy();
		return propertyWrapper;
	}
	else if( propertyType.is_derived_from< EngineObject >() )
	{
		auto propertyWrapper = gcnew ObjectPropertyWrapper( property );
		propertyWrapper->BuildHierarchy();
		return propertyWrapper;
	}
	else
		throw gcnew System::NotImplementedException();
}

/**@brief Zbuduj hierarchiê metadanych z podanego obiektu.*/
void ObjectPropertyWrapper::BuildHierarchy()
{
	BuildHierarchy( RTTRPropertyRapist::MakeProperty( m_metaProperty ).get_type() );
}

//====================================================================================//
//				CategoryLessPropertyWrapper
//====================================================================================//

/**@brief Buduje hierarchiê dla typów DirectX::XMFLOAT...

Funkcja nie grupuje Property w kategorie w przeciwieñstwie do
@ref CategoryPropertyWrapper::BuildHierarchy.
*/
void CategoryLessPropertyWrapper::BuildHierarchy( rttr::type classType )
{
	classType = classType.get_raw_type();
	auto properties = classType.get_properties();

	for( auto& prop : properties )
	{
		Properties->Add( BuildProperty( prop ) );
	}
}

/**@brief Zbuduj hierarchiê metadanych z podanego obiektu.*/
void CategoryLessPropertyWrapper::BuildHierarchy()
{
	BuildHierarchy( RTTRPropertyRapist::MakeProperty( m_metaProperty ).get_type() );
}

/**@brief */
void	CategoryLessPropertyWrapper::ResetActor		( System::IntPtr objectPtr )
{
	auto prop = RTTRPropertyRapist::MakeProperty( m_metaProperty );

	if( m_type == PropertyType::PropertyFloat2 )
	{
		auto value = prop.get_value( *static_cast< EngineObject* >( objectPtr.ToPointer() ) );
		auto xmfloat = value.get_value< DirectX::XMFLOAT2* >();

		for each( auto property in Properties )
			property->ResetActor( System::IntPtr( (void*)xmfloat ) );
	}
	else if( m_type == PropertyType::PropertyFloat3 )
	{
		auto value = prop.get_value( *static_cast< EngineObject* >( objectPtr.ToPointer() ) );
		auto xmfloat = value.get_value< DirectX::XMFLOAT3* >();

		for each( auto property in Properties )
			property->ResetActor( System::IntPtr( (void*)xmfloat ) );
	}
	else if( m_type == PropertyType::PropertyFloat4 )
	{
		auto value = prop.get_value( *static_cast< EngineObject* >( objectPtr.ToPointer() ) );
		auto xmfloat = value.get_value< DirectX::XMFLOAT4* >();

		for each( auto property in Properties )
			property->ResetActor( System::IntPtr( (void*)xmfloat ) );
	}
	else if( m_type == PropertyType::PropertyResource || m_type == PropertyType::PropertyActor )
	{
		auto value = prop.get_value( *static_cast< EngineObject* >( objectPtr.ToPointer() ) );
		auto object = value.get_value< EngineObject* >();

		for each( auto property in Properties )
			property->ResetActor( System::IntPtr( (void*)object ) );
	}
	else
		assert( false );
	
}


}
