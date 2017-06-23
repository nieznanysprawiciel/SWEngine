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



/**@brief Base class for all property wrappers.*/
public ref class PropertyWrapper
{
protected:

	System::String^			m_name;
	PropertyType			m_type;
	bool					m_expandProperty;

	void*					m_actorPtr;		///< WskaŸnik u¿ywany do pobierania parametrów obiektu klasy.
	PropertyWrapper^		m_parent;

protected:

	const rttr::detail::property_wrapper_base*			m_metaProperty;

public:
	PropertyWrapper( void* parent, PropertyType type, rttr::property prop, const char* name )
		: m_type( type )
		, m_actorPtr( parent )
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


//====================================================================================//
//			Generic object wrapper	
//====================================================================================//




/**@brief Property dla typów, które nie grupuj¹ swoich w³aœciwoœci w podkategorie
tylko wyœwietlaj¹ je bezpoœrednio.*/
public ref class CategoryLessPropertyWrapper : CategoryWrapper
{
private:
protected:
	CategoryLessPropertyWrapper( void* parent, PropertyType type, rttr::property prop, const char* name )
		: CategoryWrapper( parent, type, prop, name )
	{}

public:
	CategoryLessPropertyWrapper( void* parent, const char* name )
		: CategoryWrapper( parent, name )
	{}

	void			BuildHierarchy	( void* parent, rttr::type classType ) override;
	void			BuildHierarchy	();

};



/**@brief Property dla obiektów z³o¿onych dziedzicz¹cych po EngineObject.*/
public ref class ObjectPropertyWrapper : CategoryLessPropertyWrapper
{
private:
public:
	ObjectPropertyWrapper( void* parent, rttr::property prop )
		: CategoryLessPropertyWrapper( parent, PropertyType::PropertyActor, prop, prop.get_name().to_string().c_str() )
	{}
};


/**@brief Property typu DirectX::XMFLOAT2, XMFLOAT3, XMFLOAT4.*/
public ref class XMFloatPropertyWrapper : CategoryLessPropertyWrapper
{
private:
public:
	XMFloatPropertyWrapper( void* parent, rttr::property prop )
		: CategoryLessPropertyWrapper( parent, GetPropertyType( prop ), prop, prop.get_name().to_string().c_str() )
	{}
};



}
}	// sw
