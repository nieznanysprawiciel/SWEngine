/**
@file BasicTypesWrappers.cpp
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/


#include "EngineEditor/PropertyWrapperRTTR/stdafx.h"

#include "BasicTypesWrappers.h"



namespace sw {
namespace EditorPlugin
{


//====================================================================================//
//				IntPropertyWrapper	
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

//====================================================================================//
//				UIntPropertyWrapper
//====================================================================================//

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


//====================================================================================//
//				BoolPropertyWrapper
//====================================================================================//

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


//====================================================================================//
//				FloatPropertyWrapper
//====================================================================================//

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

//====================================================================================//
//				DoublePropertyWrapper
//====================================================================================//

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



}
}	// sw

