/**
@file EnumPropertyWrapper.cpp
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/

#include "EngineEditor/PropertyWrapperRTTR/stdafx.h"
#include "EnumPropertyWrapper.h"
#include "EngineEditor/PropertyWrapperRTTR/Properties/Complex/HierarchicalPropertyWrapper.h"

#include <msclr/marshal_cppstd.h>



namespace sw {
namespace EditorPlugin
{


// ================================ //
//
EnumPropertyWrapper::EnumPropertyWrapper		( HierarchicalPropertyWrapper^ parent, rttr::property prop )
	: PropertyWrapper( parent, PropertyType::PropertyEnum, prop, prop.get_name().to_string().c_str() )
{
	Init( prop );
}

// ================================ //
//
EnumPropertyWrapper::EnumPropertyWrapper		( HierarchicalPropertyWrapper ^ parent, rttr::property prop, const char * name )
	: PropertyWrapper( parent, PropertyType::PropertyEnum, prop, name )
{
	Init( prop );
}

// ================================ //
//
void				EnumPropertyWrapper::Init				( rttr::property& prop )
{
	auto enumeration = prop.get_enumeration();
	auto enumStrings = enumeration.get_names();

	m_enumStrings = gcnew List< System::String^ >();
	for each( auto& string in enumStrings )
	{
		m_enumStrings->Add( gcnew System::String( string.to_string().c_str() ) );
	}
}


// ================================ //
//
System::String^		EnumPropertyWrapper::Value::get			()
{
	return GetValue( m_parent->GetWrappedObject() );
}


// ================================ //
//
void				EnumPropertyWrapper::Value::set			( System::String^ newValue )
{
	auto instance = m_parent->GetWrappedObject();
	SetValue( instance, newValue );
}

// ================================ //
//
System::String^		EnumPropertyWrapper::GetValue			( const rttr::instance& refObject )
{
	rttr::property prop = RTTRPropertyRapist::MakeProperty( m_metaProperty );
	rttr::variant enumValue = prop.get_value( refObject );

	auto enumeration = prop.get_enumeration();
	return gcnew System::String( enumeration.value_to_name( enumValue ).to_string().c_str() );
}


// ================================ //
//
void				EnumPropertyWrapper::SetValue			( rttr::instance& refObject, System::String^ newValue )
{
	rttr::property prop = RTTRPropertyRapist::MakeProperty( m_metaProperty );

	auto enumeration = prop.get_enumeration();
	rttr::variant enumValue = enumeration.name_to_value( msclr::interop::marshal_as< std::string >( newValue ) );

	prop.set_value( refObject, enumValue );
}



}
}

