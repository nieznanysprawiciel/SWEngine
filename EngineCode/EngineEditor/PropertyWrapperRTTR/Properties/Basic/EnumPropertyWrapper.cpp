/**
@file EnumPropertyWrapper.cpp
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/

#include "EngineEditor/PropertyWrapperRTTR/stdafx.h"
#include "EnumPropertyWrapper.h"

#include <msclr/marshal_cppstd.h>



namespace sw {
namespace EditorPlugin
{


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

}
}

