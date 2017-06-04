/**
@file ArrayPropertyWrapper.cpp
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/


#include "ArrayPropertyWrapper.h"
#include "ArrayElementPropertyWrapper.h"


namespace sw {
namespace EditorPlugin
{

// ================================ //
//
ArrayPropertyWrapper::ArrayPropertyWrapper( void* parent, rttr::property prop )
	: CategoryLessPropertyWrapper( parent, PropertyType::PropertyArray, prop, prop.get_name().c_str() )
{
	assert( prop.is_array() );

	rttr::variant declaringObject( parent );
	bool success = declaringObject.unsafe_convert_void( prop.get_declaring_type_ptr() );

	assert( success );

	rttr::variant arrayVariant = prop.get_value( declaringObject );

	auto array = arrayVariant.create_array_view();
	m_arraySize = (uint32)array.get_size();
	m_isDynamic = array.is_dynamic();

	assert( array.get_rank() == 1 );

	m_expandProperty = false;
}

// ================================ //
//
void ArrayPropertyWrapper::BuildHierarchy( void* parent, rttr::type classType )
{
	m_actorPtr = parent;
	rttr::property prop = RTTRPropertyRapist::MakeProperty( m_metaProperty );

	rttr::variant declaringObject( parent );
	bool success = declaringObject.unsafe_convert_void( prop.get_declaring_type_ptr() );

	assert( success );

	rttr::variant arrayVariant = prop.get_value( declaringObject );

	auto array = arrayVariant.create_array_view();

	assert( array.get_size() == m_arraySize );

	for( uint32 i = 0; i < m_arraySize; ++i )
	{
		rttr::variant valuePtr = array.get_value_as_ptr( i );
		assert( valuePtr.is_valid() );

		ArrayElementPropertyWrapper^ arrayElement = gcnew ArrayElementPropertyWrapper( valuePtr.get_value< void* >(), prop.get_name() + "[ " + std::to_string( i ) + " ]" );
		arrayElement->BuildHierarchy( valuePtr.get_type() );
		Properties->Add( arrayElement );
	}
}

// ================================ //
//
void ArrayPropertyWrapper::BuildHierarchy()
{
	auto type = RTTRPropertyRapist::MakeProperty( m_metaProperty ).get_type();
	BuildHierarchy( m_actorPtr, type );
}


// ================================ //
//
bool		ArrayPropertyWrapper::IsDynamic::get()
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

