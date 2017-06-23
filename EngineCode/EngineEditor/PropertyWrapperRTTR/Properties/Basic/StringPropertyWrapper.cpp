/**
@file StringPropertyWrapper.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/

#include "EngineEditor/PropertyWrapperRTTR/stdafx.h"
#include "StringPropertyWrapper.h"

#include <msclr/marshal_cppstd.h>


namespace sw {
namespace EditorPlugin
{


//====================================================================================//
//				StringPropertyWrapper
//====================================================================================//


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


//====================================================================================//
//				WStringPropertyWrapper
//====================================================================================//



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



}
}

