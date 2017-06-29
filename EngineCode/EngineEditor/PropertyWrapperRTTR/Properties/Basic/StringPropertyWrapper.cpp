/**
@file StringPropertyWrapper.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/

#include "EngineEditor/PropertyWrapperRTTR/stdafx.h"
#include "StringPropertyWrapper.h"
#include "EngineEditor/PropertyWrapperRTTR/Properties/Complex/HierarchicalPropertyWrapper.h"

#include <msclr/marshal_cppstd.h>


namespace sw {
namespace EditorPlugin
{


//====================================================================================//
//				StringPropertyWrapper
//====================================================================================//


// ================================ //
//
System::String^		StringPropertyWrapper::Value::get	()
{
	auto value = GetPropertyValue< std::string >( m_metaProperty, m_parent->GetWrappedObject() );
	return gcnew System::String( value.c_str() );
}

// ================================ //
//
void				StringPropertyWrapper::Value::set	( System::String^ value )
{
	auto instance = m_parent->GetWrappedObject();
	SetPropertyValue< std::string >( m_metaProperty, instance, msclr::interop::marshal_as< std::string >( value ) );
}


//====================================================================================//
//				WStringPropertyWrapper
//====================================================================================//


// ================================ //
//
System::String^		WStringPropertyWrapper::Value::get	()
{
	auto value = GetPropertyValue< std::wstring >( m_metaProperty, m_parent->GetWrappedObject() );
	return gcnew System::String( value.c_str() );
}

// ================================ //
//
void				WStringPropertyWrapper::Value::set	( System::String^ value )
{
	auto instance = m_parent->GetWrappedObject();
	SetPropertyValue< std::wstring >( m_metaProperty, instance, msclr::interop::marshal_as< std::wstring >( value ) );
}




}
}

