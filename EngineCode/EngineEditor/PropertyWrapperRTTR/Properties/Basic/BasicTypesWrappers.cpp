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
	return GetTypedValue< int >();
}

// ================================ //
//
void		IntPropertyWrapper::Value::set		( int newValue )
{
	SetTypedValue< int >( newValue );
}


//====================================================================================//
//				UIntPropertyWrapper
//====================================================================================//


// ================================ //
//
uint32		UIntPropertyWrapper::Value::get		()
{
	return GetTypedValue< uint32 >();
}

// ================================ //
//
void		UIntPropertyWrapper::Value::set		( uint32 newValue )
{
	SetTypedValue< uint32 >( newValue );
}


//====================================================================================//
//				BoolPropertyWrapper
//====================================================================================//



// ================================ //
//
bool		BoolPropertyWrapper::Value::get		()
{
	return GetTypedValue< bool >();
}

// ================================ //
//
void		BoolPropertyWrapper::Value::set		( bool newValue )
{
	SetTypedValue< bool >( newValue );
}


//====================================================================================//
//				FloatPropertyWrapper
//====================================================================================//



// ================================ //
//
float		FloatPropertyWrapper::Value::get		()
{
	return GetTypedValue< float >();
}

// ================================ //
//
void		FloatPropertyWrapper::Value::set		( float newValue )
{
	SetTypedValue< float >( newValue );
}


//====================================================================================//
//				DoublePropertyWrapper
//====================================================================================//



// ================================ //
//
double		DoublePropertyWrapper::Value::get		()
{
	return GetTypedValue< double >();
}

// ================================ //
//
void		DoublePropertyWrapper::Value::set		( double newValue )
{
	SetTypedValue< double >( newValue );
}




}
}	// sw

