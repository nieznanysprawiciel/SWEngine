#pragma once
/**
@file PropertyWrapper.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/


#include "PropertyHelper.h"
#include "swCommonLib/Common/EngineObject.h"
#include "swGraphicAPI/Resources/ResourceObject.h"


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


	PropertyType			GetPropertyType	( rttr::property prop )
	{
		auto propertyType = prop.get_type();
		if( propertyType == rttr::type::get< int >() )
			return PropertyType::PropertyInt;
		else if( propertyType == rttr::type::get< float >() )
			return PropertyType::PropertyFloat;
		else if( propertyType == rttr::type::get< bool >() )
			return PropertyType::PropertyBool;
		else if( propertyType == rttr::type::get< double >() )
			return PropertyType::PropertyDouble;
		else if( propertyType == rttr::type::get< DirectX::XMFLOAT2* >() )
			return PropertyType::PropertyFloat2;
		else if( propertyType == rttr::type::get< DirectX::XMFLOAT3* >() )
			return PropertyType::PropertyFloat3;
		else if( propertyType == rttr::type::get< DirectX::XMFLOAT4* >() )
			return PropertyType::PropertyFloat4;
		else if( propertyType.is_derived_from< EngineObject >() )
			return PropertyType::PropertyActor;
		else if( propertyType.is_derived_from< ResourceObject >() )
			return PropertyType::PropertyResource;
		else if( propertyType == rttr::type::get< std::string >() )
			return PropertyType::PropertyString;
		else if( propertyType == rttr::type::get< std::wstring >() )
			return PropertyType::PropertyWString;
		else
			return PropertyType::PropertyUnknown;
	}

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
