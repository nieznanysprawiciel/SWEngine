/**
@file BasicTypesWrappers.cpp
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/


#include "EngineEditor/PropertyWrapperRTTR/stdafx.h"

#include "BasicTypesWrappers.h"
#include "EngineEditor/PropertyWrapperRTTR/Properties/Complex/HierarchicalPropertyWrapper.h"



namespace sw {
namespace EditorPlugin
{


//====================================================================================//
//				IntPropertyWrapper	
//====================================================================================//

// ================================ //
//
int			IntPropertyWrapper::Value::get		()
{
	auto value = GetPropertyValue< int >( m_metaProperty, m_parent->GetWrappedObject() );
	return value;
}

// ================================ //
//
void		IntPropertyWrapper::Value::set		( int newValue )
{
	auto instance = m_parent->GetWrappedObject();
	SetPropertyValue< int >( m_metaProperty, instance, newValue );
}


//====================================================================================//
//				UIntPropertyWrapper
//====================================================================================//


// ================================ //
//
uint32		UIntPropertyWrapper::Value::get		()
{
	auto value = GetPropertyValue< uint32 >( m_metaProperty, m_parent->GetWrappedObject() );
	return value;
}

// ================================ //
//
void		UIntPropertyWrapper::Value::set		( uint32 newValue )
{
	auto instance = m_parent->GetWrappedObject();
	SetPropertyValue< uint32 >( m_metaProperty, instance, newValue );
}


//====================================================================================//
//				BoolPropertyWrapper
//====================================================================================//



// ================================ //
//
bool		BoolPropertyWrapper::Value::get		()
{
	auto value = GetPropertyValue< bool >( m_metaProperty, m_parent->GetWrappedObject() );
	return value;
}

// ================================ //
//
void		BoolPropertyWrapper::Value::set		( bool newValue )
{
	auto instance = m_parent->GetWrappedObject();
	SetPropertyValue< bool >( m_metaProperty, instance, newValue );
}


//====================================================================================//
//				FloatPropertyWrapper
//====================================================================================//



// ================================ //
//
float		FloatPropertyWrapper::Value::get		()
{
	auto value = GetPropertyValue< float >( m_metaProperty, m_parent->GetWrappedObject() );
	return value;
}

// ================================ //
//
void		FloatPropertyWrapper::Value::set		( float newValue )
{
	auto instance = m_parent->GetWrappedObject();
	SetPropertyValue< float >( m_metaProperty, instance, newValue );
}


//====================================================================================//
//				DoublePropertyWrapper
//====================================================================================//



// ================================ //
//
double		DoublePropertyWrapper::Value::get		()
{
	auto value = GetPropertyValue< double >( m_metaProperty, m_parent->GetWrappedObject() );
	return value;
}

// ================================ //
//
void		DoublePropertyWrapper::Value::set		( double newValue )
{
	auto instance = m_parent->GetWrappedObject();
	SetPropertyValue< double >( m_metaProperty, instance, newValue );
}




}
}	// sw

