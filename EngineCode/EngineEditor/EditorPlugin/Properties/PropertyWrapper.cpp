#pragma warning(disable : 4561)
#include "PropertyWrapper.h"
#include <msclr/marshal_cppstd.h>

#include "ResourcePropertyWrapper.h"



namespace EditorPlugin
{


//====================================================================================//
//			PropertyWrapper implementation	
//====================================================================================//


/**@brief */
void			PropertyWrapper::ResetActor		( System::IntPtr objectPtr )
{
	m_actorPtr = objectPtr.ToPointer();
}


//====================================================================================//
//			Regular Properties	
//====================================================================================//


/**@brief */
int IntPropertyWrapper::GetValue( void* refObject )
{
	auto value = GetPropertyValue< int >( m_metaProperty, System::IntPtr( refObject ) );
	return value;
}

/**@brief */
void IntPropertyWrapper::SetValue( void* refObject, int newValue )
{
	SetPropertyValue< int >( m_metaProperty, System::IntPtr( refObject ), newValue );
}


/**@brief */
bool BoolPropertyWrapper::GetValue( void* refObject )
{
	auto value = GetPropertyValue< bool >( m_metaProperty, System::IntPtr( refObject ) );
	return value;
}

/**@brief */
void BoolPropertyWrapper::SetValue( void* refObject, bool newValue )
{
	SetPropertyValue< bool >( m_metaProperty, System::IntPtr( refObject ), newValue );
}


/**@brief */
float FloatPropertyWrapper::GetValue( void* refObject )
{
	auto value = GetPropertyValue< float >( m_metaProperty, System::IntPtr( refObject ) );
	return value;
}

/**@brief */
void FloatPropertyWrapper::SetValue( void* refObject, float newValue )
{
	SetPropertyValue< float >( m_metaProperty, System::IntPtr( refObject ), newValue );
}

/**@brief */
double DoublePropertyWrapper::GetValue( void* refObject )
{
	auto value = GetPropertyValue< double >( m_metaProperty, System::IntPtr( refObject ) );
	return value;
}

/**@brief */
void DoublePropertyWrapper::SetValue( void* refObject, double newValue )
{
	SetPropertyValue< double >( m_metaProperty, System::IntPtr( refObject ), newValue );
}

/**@brief */
System::String ^ StringPropertyWrapper::GetValue( void* refObject )
{
	auto value = GetPropertyValue< std::string >( m_metaProperty, System::IntPtr( refObject ) );
	return gcnew System::String( value.c_str() );
}

/**@brief */
void StringPropertyWrapper::SetValue( void* refObject, System::String^ newValue )
{
	SetPropertyValue< std::string >( m_metaProperty, System::IntPtr( refObject ), msclr::interop::marshal_as< std::string >( newValue ) );
}

/**@brief */
System::String ^ WStringPropertyWrapper::GetValue( void* refObject )
{
	auto value = GetPropertyValue< std::wstring >( m_metaProperty, System::IntPtr( refObject ) );
	return gcnew System::String( value.c_str() );
}

/**@brief */
void WStringPropertyWrapper::SetValue( void* refObject, System::String^ newValue )
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
		auto propertyWrapper = gcnew ResourcePropertyWrapper( property );
		propertyWrapper->BuildHierarchy();
		return propertyWrapper;
	}
	else if( propertyType.is_derived_from< EngineObject >() )
	{
		auto propertyWrapper = gcnew ObjectPropertyWrapper( property );
		propertyWrapper->BuildHierarchy();
		return propertyWrapper;
	}
	else if ( propertyType.is_pointer() )
	{
		// All other objects which use BindAsPointer policy.
		throw gcnew System::NotImplementedException();
	}
	else
		throw gcnew System::ArgumentException( gcnew System::String( "Property type: [" )
											   + gcnew System::String( property.get_name().c_str() )
											   + gcnew System::String( "] is not supported" ),
											   gcnew System::String( "property" ) );
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
void	CategoryLessPropertyWrapper::ResetActor		( System::IntPtr parentObjectPtr )
{
	auto prop = RTTRPropertyRapist::MakeProperty( m_metaProperty );

	if( m_type == PropertyType::PropertyFloat2 )
	{
		auto value = prop.get_value( *static_cast< EngineObject* >( parentObjectPtr.ToPointer() ) );
		auto xmfloat = value.get_value< DirectX::XMFLOAT2* >();

		for each( auto property in Properties )
			property->ResetActor( System::IntPtr( (void*)xmfloat ) );
	}
	else if( m_type == PropertyType::PropertyFloat3 )
	{
		auto value = prop.get_value( *static_cast< EngineObject* >( parentObjectPtr.ToPointer() ) );
		auto xmfloat = value.get_value< DirectX::XMFLOAT3* >();

		for each( auto property in Properties )
			property->ResetActor( System::IntPtr( (void*)xmfloat ) );
	}
	else if( m_type == PropertyType::PropertyFloat4 )
	{
		auto value = prop.get_value( *static_cast< EngineObject* >( parentObjectPtr.ToPointer() ) );
		auto xmfloat = value.get_value< DirectX::XMFLOAT4* >();

		for each( auto property in Properties )
			property->ResetActor( System::IntPtr( (void*)xmfloat ) );
	}
	else if( m_type == PropertyType::PropertyResource || m_type == PropertyType::PropertyActor )
	{
		auto value = prop.get_value( *static_cast< EngineObject* >( parentObjectPtr.ToPointer() ) );
		auto object = value.get_value< EngineObject* >();

		for each( auto property in Properties )
			property->ResetActor( System::IntPtr( (void*)object ) );
	}
	else
		assert( false );
	
}


}
