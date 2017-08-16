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

	HierarchicalPropertyWrapper^	m_parent;

protected:

	const rttr::detail::property_wrapper_base*			m_metaProperty;

public:
	PropertyWrapper( HierarchicalPropertyWrapper^ parent, PropertyType type, rttr::property prop, const char* name )
		: m_type( type )
		, m_parent( parent )
		, m_expandProperty( true )
	{
		m_metaProperty = RTTRPropertyRapist::GetWrapperBase( prop );
		m_name = gcnew System::String( name );
	}


	PropertyType			GetPropertyType	( rttr::property prop );

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
};



}
}	// sw
