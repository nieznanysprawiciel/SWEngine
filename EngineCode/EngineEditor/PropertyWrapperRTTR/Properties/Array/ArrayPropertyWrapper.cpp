/**
@file ArrayPropertyWrapper.cpp
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/
#include "EngineEditor/PropertyWrapperRTTR/stdafx.h"

#include "ArrayPropertyWrapper.h"
#include "ArrayElementPropertyWrapper.h"

#include "swCommonLib/Common/Properties/Properties.h"


namespace sw {
namespace EditorPlugin
{

// ================================ //
//
ArrayPropertyWrapper::ArrayPropertyWrapper( HierarchicalPropertyWrapper^ parent, rttr::property prop )
	: HierarchicalPropertyWrapper( parent, PropertyType::PropertyArray, prop, prop.get_name().to_string().c_str() )
{
	assert( prop.is_array() );

	auto parentInstance = parent->GetWrappedObject();
	rttr::variant arrayVariant = prop.get_value( parentInstance );

	bool isValid = arrayVariant.is_valid();

	auto array = arrayVariant.create_array_view();
	m_arraySize = (uint32)array.get_size();
	m_isDynamic = array.is_dynamic();

	assert( array.get_rank() == 1 );

	m_expandProperty = false;
}

// ================================ //
//
void				ArrayPropertyWrapper::BuildHierarchy		( rttr::type classType, BuildContext& context )
{
	rttr::property prop = RTTRPropertyRapist::MakeProperty( m_metaProperty );
	rttr::variant arrayVariant = prop.get_value( m_parent->GetWrappedObject() );

	SetGenericValue( arrayVariant );
	auto array = arrayVariant.create_array_view();

	assert( array.get_size() == m_arraySize );

	for( uint32 i = 0; i < m_arraySize; ++i )
	{
		rttr::variant valueRef = array.get_value_as_ref( i );
		rttr::instance valueInstance = valueRef;
		assert( valueRef.is_valid() );

		ArrayElementPropertyWrapper^ arrayElement = gcnew ArrayElementPropertyWrapper( this, prop.get_name().to_string() + "[ " + std::to_string( i ) + " ]" );
		arrayElement->BuildHierarchy( ::Properties::GetRealWrappedType( valueInstance ), context );
		AddPropertyChild( arrayElement );
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

