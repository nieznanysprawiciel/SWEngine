/**
@file ArrayPropertyWrapper.cpp
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/
#include "EngineEditor/PropertyWrapperRTTR/stdafx.h"

#include "ArrayPropertyWrapper.h"

#include "EngineEditor/PropertyWrapperRTTR/Build/PropertyBuilder.h"
#include "swCommonLib/Common/Properties/Properties.h"

#include <msclr/marshal_cppstd.h>





namespace sw {
namespace EditorPlugin
{

// ================================ //
//
ArrayPropertyWrapper::ArrayPropertyWrapper		( HierarchicalPropertyWrapper^ parent, rttr::property prop )
	: HierarchicalPropertyWrapper( parent, PropertyType::PropertyArray, prop, prop.get_name().to_string().c_str() )
{
	Init( parent, prop );
}

// ================================ //
//
ArrayPropertyWrapper::ArrayPropertyWrapper		( HierarchicalPropertyWrapper^ parent, rttr::property prop, const char* name )
	: HierarchicalPropertyWrapper( parent, PropertyType::PropertyArray, prop, name )
{
	Init( parent, prop );
}

// ================================ //
//
void				ArrayPropertyWrapper::Init					( HierarchicalPropertyWrapper^ parent, rttr::property prop )
{
	assert( prop.is_array() );

	auto parentInstance = parent->GetWrappedObject();
	rttr::variant arrayVariant = prop.get_value( parentInstance );

	bool isValid = arrayVariant.is_valid();

	auto array = arrayVariant.create_array_view();
	m_arraySize = (uint32)array.get_size();
	m_isDynamic = array.is_dynamic();

	//assert( array.get_rank() == 1 );

	m_expandProperty = false;
}

// ================================ //
//
void				ArrayPropertyWrapper::RebuildProperty		( rttr::variant& parent, BuildContext& context )
{
	rttr::variant thisVariant = RecomputeObject( parent );

	SetGenericValue( thisVariant );
	RebuildProperties( thisVariant, context );
}

// ================================ //
//
void				ArrayPropertyWrapper::RebuildProperties		( rttr::variant& arrayVariant, BuildContext& context )
{
	auto array = arrayVariant.create_array_view();

	auto prevSize = m_properties->Count;
	m_arraySize = (uint32)array.get_size();
	m_isDynamic = array.is_dynamic();

	m_properties->Clear();

	/// @todo Not all elements should be updated on RebuildProperties call.

	std::string arrayName = msclr::interop::marshal_as< std::string >( PropertyName );

	for( uint32 i = 0; i < m_arraySize; ++i )
	{
		rttr::variant valueRef = array.get_value_as_ref( i );
		rttr::instance valueInstance = valueRef;
		assert( valueRef.is_valid() );

		
		std::string elementName = arrayName + "[ " + std::to_string( i ) + " ]";

		PropertyWrapper^ element = PropertyBuilder::BuildProperty( this, valueRef, elementName, context );

		AddPropertyChild( element );
	}
}

// ================================ //
//
bool				ArrayPropertyWrapper::IsDynamic::get()
{
	return m_isDynamic;
}

// ================================ //
//
uint32		ArrayPropertyWrapper::ArraySize::get()
{
	return m_arraySize;
}


}	// EditorPlugin
}	// sw

