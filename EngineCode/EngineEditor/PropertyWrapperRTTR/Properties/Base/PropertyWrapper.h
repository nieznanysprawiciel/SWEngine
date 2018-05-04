#pragma once
/**
@file PropertyWrapper.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/




#include "EngineEditor/PropertyWrapperRTTR/Build/IncludeRTTR.h"
#include "PropertyHelper.h"



#include <memory>


class EngineObject;


namespace sw {
namespace EditorPlugin
{

using namespace System::Collections::Generic;

ref class PropertyWrapper;
ref class HierarchicalPropertyWrapper;



/**@brief Base class for all property wrappers.*/
public ref class PropertyWrapper
{
protected:

	System::String^			m_name;
	PropertyType			m_type;
	bool					m_expandProperty;
	bool					m_boundAsReference;

	rttr::variant*			m_propertyValue;

	HierarchicalPropertyWrapper^	m_parent;

protected:

	const rttr::detail::property_wrapper_base*			m_metaProperty;

public:

	// ================================ //
	//
	PropertyWrapper( HierarchicalPropertyWrapper^ parent, PropertyType type, rttr::property prop, const char* name )
		: m_type( type )
		, m_parent( parent )
		, m_expandProperty( true )
		, m_propertyValue( nullptr )
		, m_boundAsReference( false )
	{
		m_metaProperty = RTTRPropertyRapist::GetWrapperBase( prop );
		m_name = gcnew System::String( name );
	}

	virtual ~PropertyWrapper();

public:

	PropertyType			GetPropertyType			( rttr::property prop );
	virtual void			RebuildProperty			( rttr::variant& parent, BuildContext& context );


	HierarchicalPropertyWrapper^		GetParent	()
	{
		return m_parent;
	}

public:

	property PropertyType		Type
	{
		PropertyType		get() { return m_type; }
	}

	property System::String^	PropertyName
	{
		System::String^		get() { return m_name; }
	}

	property bool				ExpandProperty
	{
		bool				get	() { return m_expandProperty; }
		void				set	( bool value ) { m_expandProperty = value; }
	}


protected:

	void					SetGenericValue			( const rttr::variant& value );
	rttr::variant*			GetGenericValue			();

	template< typename PropertyType >
	void					SetTypedValue			( PropertyType value );

	template< typename PropertyType >
	PropertyType			GetTypedValue			();

	template< typename PropertyType >
	void					SetBoundByReference		( TypeID type );
};


//====================================================================================//
//			Implementation
//====================================================================================//

// ================================ //
//
template< typename PropertyType >
inline void				PropertyWrapper::SetTypedValue		( PropertyType value )
{
	if( m_parent && m_metaProperty )
	{
		auto instance = m_parent->GetWrappedObject();
		SetPropertyValue< PropertyType >( m_metaProperty, instance, value );
		SetGenericValue( value );
	}
	else if( m_boundAsReference && GetGenericValue() != nullptr )
	{
		auto genericValue = *GetGenericValue();
		auto & valueReference = genericValue.get_value< std::reference_wrapper< PropertyType > >();

		valueReference.get() = value;
	}
}

// ================================ //
//
template< typename PropertyType >
inline PropertyType		PropertyWrapper::GetTypedValue		()
{
	return GetPropertyValue< PropertyType >( GetGenericValue() );
}

// ================================ //
//
template< typename PropertyType >
inline void				PropertyWrapper::SetBoundByReference	( TypeID type )
{
	if( type == TypeID::get< std::reference_wrapper< PropertyType > >() )
		m_boundAsReference = true;
}

}
}	// sw
