#pragma warning(disable : 4561)
#include "EngineEditor/PropertyWrapperRTTR/stdafx.h"

#include "PropertyWrapper.h"
#include "swCommonLib/Common/RTTR.h"

#include <msclr/marshal_cppstd.h>

#include "ResourcePropertyWrapper.h"
#include "ArrayPropertyWrapper.h"



namespace sw {
namespace EditorPlugin
{


//====================================================================================//
//			PropertyWrappers implementation	
//====================================================================================//



//====================================================================================//
//			Regular Properties	
//====================================================================================//


/**@brief */
int			IntPropertyWrapper::GetValue( void* refObject )
{
	auto value = GetPropertyValue< int >( m_metaProperty, System::IntPtr( refObject ) );
	return value;
}

/**@brief */
void		IntPropertyWrapper::SetValue( void* refObject, int newValue )
{
	SetPropertyValue< int >( m_metaProperty, System::IntPtr( refObject ), newValue );
}

/**@brief */
uint32		UIntPropertyWrapper::GetValue( void* refObject )
{
	auto value = GetPropertyValue< uint32 >( m_metaProperty, System::IntPtr( refObject ) );
	return value;
}

/**@brief */
void		UIntPropertyWrapper::SetValue( void* refObject, uint32 newValue )
{
	SetPropertyValue< uint32 >( m_metaProperty, System::IntPtr( refObject ), newValue );
}


/**@brief */
bool		BoolPropertyWrapper::GetValue( void* refObject )
{
	auto value = GetPropertyValue< bool >( m_metaProperty, System::IntPtr( refObject ) );
	return value;
}

/**@brief */
void		BoolPropertyWrapper::SetValue( void* refObject, bool newValue )
{
	SetPropertyValue< bool >( m_metaProperty, System::IntPtr( refObject ), newValue );
}


/**@brief */
float		FloatPropertyWrapper::GetValue( void* refObject )
{
	auto value = GetPropertyValue< float >( m_metaProperty, System::IntPtr( refObject ) );
	return value;
}

/**@brief */
void		FloatPropertyWrapper::SetValue( void* refObject, float newValue )
{
	SetPropertyValue< float >( m_metaProperty, System::IntPtr( refObject ), newValue );
}

/**@brief */
double		DoublePropertyWrapper::GetValue( void* refObject )
{
	auto value = GetPropertyValue< double >( m_metaProperty, System::IntPtr( refObject ) );
	return value;
}

/**@brief */
void		DoublePropertyWrapper::SetValue( void* refObject, double newValue )
{
	SetPropertyValue< double >( m_metaProperty, System::IntPtr( refObject ), newValue );
}

/**@brief */
System::String^		StringPropertyWrapper::GetValue( void* refObject )
{
	auto value = GetPropertyValue< std::string >( m_metaProperty, System::IntPtr( refObject ) );
	return gcnew System::String( value.c_str() );
}

/**@brief */
void				StringPropertyWrapper::SetValue( void* refObject, System::String^ newValue )
{
	SetPropertyValue< std::string >( m_metaProperty, System::IntPtr( refObject ), msclr::interop::marshal_as< std::string >( newValue ) );
}

/**@brief */
System::String^		WStringPropertyWrapper::GetValue( void* refObject )
{
	auto value = GetPropertyValue< std::wstring >( m_metaProperty, System::IntPtr( refObject ) );
	return gcnew System::String( value.c_str() );
}

/**@brief */
void				WStringPropertyWrapper::SetValue( void* refObject, System::String^ newValue )
{
	SetPropertyValue< std::wstring >( m_metaProperty, System::IntPtr( refObject ), msclr::interop::marshal_as< std::wstring >( newValue ) );
}

//====================================================================================//
//			EnumPropertyWrapper	
//====================================================================================//

/**@brief */
EnumPropertyWrapper::EnumPropertyWrapper( void* parent, rttr::property prop )
	: PropertyWrapper( parent, PropertyType::PropertyEnum, prop, prop.get_name().to_string().c_str() )
{
	auto enumeration = prop.get_enumeration();
	auto enumStrings = enumeration.get_names();

	m_enumStrings = gcnew List< System::String^ >();
	for each( auto& string in enumStrings )
	{
		m_enumStrings->Add( gcnew System::String( string.to_string().c_str() ) );
	}
}

/**@brief */
System::String^		EnumPropertyWrapper::GetValue( void* refObject )
{
	rttr::property prop = RTTRPropertyRapist::MakeProperty( m_metaProperty );

	// Create variant with void* type and convert it to proper type.
	rttr::variant declaringObject( refObject );
	bool success = declaringObject.unsafe_convert_void( prop.get_declaring_type_ptr() );

	assert( success );

	rttr::variant enumValue = prop.get_value( declaringObject );

	auto enumeration = prop.get_enumeration();
	return gcnew System::String( enumeration.value_to_name( enumValue ).to_string().c_str() );
}

/**@brief */
void				EnumPropertyWrapper::SetValue( void* refObject, System::String^ newValue )
{
	rttr::property prop = RTTRPropertyRapist::MakeProperty( m_metaProperty );

	// Create variant with void* type and convert it to proper type.
	rttr::variant declaringObject( refObject );
	bool success = declaringObject.unsafe_convert_void( prop.get_declaring_type_ptr() );

	assert( success );

	auto enumeration = prop.get_enumeration();
	rttr::variant enumValue = enumeration.name_to_value( msclr::interop::marshal_as< std::string >( newValue ) );

	prop.set_value( declaringObject, enumValue );
}


//====================================================================================//
//			ObjectPropertyWrapper	CategoryPropertyWrapper
//====================================================================================//


/**@brief */
void CategoryPropertyWrapper::BuildHierarchy( void* parentPtr, rttr::type classType )
{
	m_actorPtr = parentPtr;

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
				categories[ categoryNameStr ] = gcnew CategoryPropertyWrapper( parentPtr, categoryNameStdStr.c_str() );

			categories[ categoryNameStr ]->Properties->Add( BuildProperty( parentPtr, prop ) );
		}
		else
		{
			if( !categories.ContainsKey( "Other" ) )
				categories[ "Other" ] = gcnew CategoryPropertyWrapper( parentPtr, "Other" );

			categories[ "Other" ]->Properties->Add( BuildProperty( parentPtr, prop ) );
		}
	}

	for each ( auto var in categories )
		m_properties->Add( var.Value );
}

/**@brief */
PropertyWrapper^ CategoryPropertyWrapper::BuildProperty( void* parent, rttr::property property )
{
	auto propertyType = property.get_type();

	if( propertyType.is_wrapper() )
		propertyType = propertyType.get_wrapped_type();

	if( propertyType.is_arithmetic() )
	{
		if( propertyType == rttr::type::get< int >() ||
			propertyType == rttr::type::get< int32 >() ||
			propertyType == rttr::type::get< int16 >() ||
			propertyType == rttr::type::get< int8 >() )
		{
			return gcnew IntPropertyWrapper( parent, property );
		}
		if( propertyType == rttr::type::get< uint32 >() ||
			propertyType == rttr::type::get< unsigned int >() ||
			propertyType == rttr::type::get< uint16 >() ||
			propertyType == rttr::type::get< uint8 >() )
		{
			return gcnew UIntPropertyWrapper( parent, property );
		}
		else if( propertyType == rttr::type::get< float >() )
		{
			return gcnew FloatPropertyWrapper( parent, property );
		}
		else if( propertyType == rttr::type::get< bool >() )
		{
			return gcnew BoolPropertyWrapper( parent, property );
		}
		else if( propertyType == rttr::type::get< double >() )
		{
			return gcnew DoublePropertyWrapper( parent, property );
		}
		else
			throw gcnew System::ArgumentException( gcnew System::String( "Property type: [" )
												   + gcnew System::String( property.get_name().to_string().c_str() )
												   + gcnew System::String( "] is not supported" ),
												   gcnew System::String( "property" ) );
	}
	else if( propertyType == rttr::type::get< std::string >() )
	{
		return gcnew StringPropertyWrapper( parent, property );
	}
	else if( propertyType == rttr::type::get< std::wstring >() )
	{
		return gcnew WStringPropertyWrapper( parent, property );
	}
	else if( propertyType == rttr::type::get< DirectX::XMFLOAT2* >()
			 || propertyType == rttr::type::get< DirectX::XMFLOAT3* >()
			 || propertyType == rttr::type::get< DirectX::XMFLOAT4* >() )
	{
		auto propertyWrapper = gcnew XMFloatPropertyWrapper( parent, property );
		propertyWrapper->BuildHierarchy();
		return propertyWrapper;
	}
	else if( propertyType.is_derived_from< ResourceObject >() )
	{
		// Najpierw sprawdzamy czy dziedziczy po ResourceObject dopiero potem
		// EngineObject. Dziêki temu mo¿emy oddzieliæ zasoby od innych w³aœciowoœci
		// i obs³u¿yæ je inaczej.
		auto propertyWrapper = gcnew ResourcePropertyWrapper( parent, property );
		propertyWrapper->BuildHierarchy();
		return propertyWrapper;
	}
	else if( propertyType.is_derived_from< EngineObject >()
			 || propertyType.is_pointer() )
	{
		auto propertyWrapper = gcnew ObjectPropertyWrapper( parent, property );
		propertyWrapper->BuildHierarchy();
		return propertyWrapper;
	}
	else if( property.is_enumeration() )
	{
		return gcnew EnumPropertyWrapper( parent, property );
	}
	else if( property.is_array() )
	{
		auto propertyWrapper = gcnew ArrayPropertyWrapper( parent, property );
		propertyWrapper->BuildHierarchy();
		return propertyWrapper;
	}
	else
		throw gcnew System::ArgumentException( gcnew System::String( "Property type: [" )
											   + gcnew System::String( property.get_type().get_name().to_string().c_str() )
											   + gcnew System::String( "] is not supported" ),
											   gcnew System::String( "property" ) );
}


//====================================================================================//
//				CategoryLessPropertyWrapper
//====================================================================================//

/**@brief Buduje hierarchiê dla typów DirectX::XMFLOAT...

Funkcja nie grupuje Property w kategorie w przeciwieñstwie do
@ref CategoryPropertyWrapper::BuildHierarchy.
*/
void CategoryLessPropertyWrapper::BuildHierarchy( void* parent, rttr::type classType )
{
	m_actorPtr = parent;

	classType = classType.get_raw_type();
	auto properties = classType.get_properties();

	for( auto& prop : properties )
	{
		Properties->Add( BuildProperty( parent, prop ) );
	}
}


void* VoidMove( void* obj ) { return obj; }

/**@brief Zbuduj hierarchiê metadanych z podanego obiektu.*/
void CategoryLessPropertyWrapper::BuildHierarchy()
{
	auto property = RTTRPropertyRapist::MakeProperty( m_metaProperty );

	rttr::variant declaringObject( VoidMove( m_actorPtr ) );
	bool success = declaringObject.unsafe_convert_void( property.get_declaring_type_ptr() );

	assert( success );

	// Trzeba pobraæ realny type w³aœciwoœci. Mo¿e byæ tak, ¿e w³aœciwoœæ jest klas¹ bazow¹,
	// a tak my chcemy zbudowaæ hierarchiê dla klasy pochodnej.
	auto realContent = property.get_value( declaringObject );

	void* realPtr = realContent.get_value< void* >();
	rttr::type realType = realContent.get_type();

	// Obs³ugujemy type owrappowane.
	if( realContent.get_type().is_wrapper() && realPtr != nullptr )
	{
		rttr::instance realInstance( realContent );
		realPtr = realInstance.get_wrapped_instance();
		realType = realContent.get_type().get_wrapped_type();
	}

	if( realPtr != nullptr )
		BuildHierarchy( realPtr, realType );
}



}
}	// sw

