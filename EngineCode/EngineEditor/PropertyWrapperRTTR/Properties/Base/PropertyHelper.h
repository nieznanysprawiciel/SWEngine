#pragma once
/**
@file PropertyHelper.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/


#include "swCommonLib/Common/RTTR.h"
#include "swCommonLib/Common/TypesDefinitions.h"
#include <DirectXMath.h>


class EngineObject;



namespace sw {
namespace EditorPlugin
{

public enum PropertyType
{
	PropertyFloat,
	PropertyFloat2,
	PropertyFloat3,
	PropertyFloat4,
	PropertyUInt,
	PropertyInt,
	PropertyBool,
	PropertyDouble,
	PropertyActor,
	PropertyCategory,
	PropertyResource,
	PropertyString,
	PropertyWString,
	PropertyEnum,
	PropertyArray,
	PropertyUnknown
};


template< typename PropertyType >
PropertyType		GetPropertyValue( const rttr::detail::property_wrapper_base* metaProperty, const rttr::instance& refObject );

template< typename PropertyType >
void				SetPropertyValue( const rttr::detail::property_wrapper_base* metaProperty, rttr::instance& refObject, PropertyType newValue );

/**@brief Helper class to extract private property_wrapper_base from rttr::property.*/
public class RTTRPropertyRapist
{
	friend ref class PropertyWrapper;
	friend ref class FloatPropertyWrapper;
	friend ref class Float2PropertyWrapper;
	friend ref class Float3PropertyWrapper;
	friend ref class Float4PropertyWrapper;
	friend ref class IntPropertyWrapper;
	friend ref class UIntPropertyWrapper;
	friend ref class BoolPropertyWrapper;
	friend ref class ActorClassMetaInfo;
	friend ref class CategoryWrapper;
	friend ref class ObjectPropertyWrapper;
	friend ref class XMFloatPropertyWrapper;
	friend ref class HierarchicalPropertyWrapper;
	friend ref class StringPropertyWrapper;
	friend ref class WStringPropertyWrapper;
	friend ref class TexturePropertyWrapper;
	friend ref class EnumPropertyWrapper;
	friend ref class ArrayPropertyWrapper;
	friend ref class ArrayElementPropertyWrapper;

	friend DirectX::XMFLOAT2	GetPropertyValue< DirectX::XMFLOAT2 >( const rttr::detail::property_wrapper_base* metaProperty, const rttr::instance& refObject );
	friend void					SetPropertyValue< DirectX::XMFLOAT2 >( const rttr::detail::property_wrapper_base* metaProperty, rttr::instance& refObject, DirectX::XMFLOAT2 newValue );

	friend DirectX::XMFLOAT3	GetPropertyValue< DirectX::XMFLOAT3 >( const rttr::detail::property_wrapper_base* metaProperty, const rttr::instance& refObject );
	friend void					SetPropertyValue< DirectX::XMFLOAT3 >( const rttr::detail::property_wrapper_base* metaProperty, rttr::instance& refObject, DirectX::XMFLOAT3 newValue );

	friend DirectX::XMFLOAT4	GetPropertyValue< DirectX::XMFLOAT4 >( const rttr::detail::property_wrapper_base* metaProperty, const rttr::instance& refObject );
	friend void					SetPropertyValue< DirectX::XMFLOAT4 >( const rttr::detail::property_wrapper_base* metaProperty, rttr::instance& refObject, DirectX::XMFLOAT4 newValue );

	friend int					GetPropertyValue< int >( const rttr::detail::property_wrapper_base* metaProperty, const rttr::instance& refObject );
	friend void					SetPropertyValue< int >( const rttr::detail::property_wrapper_base* metaProperty, rttr::instance& refObject, int newValue );

	friend uint32				GetPropertyValue< uint32 >( const rttr::detail::property_wrapper_base* metaProperty, const rttr::instance& refObject );
	friend void					SetPropertyValue< uint32 >( const rttr::detail::property_wrapper_base* metaProperty, rttr::instance& refObject, uint32 newValue );

	friend float				GetPropertyValue< float >( const rttr::detail::property_wrapper_base* metaProperty, const rttr::instance& refObject );
	friend void					SetPropertyValue< float >( const rttr::detail::property_wrapper_base* metaProperty, rttr::instance& refObject, float newValue );

	friend double				GetPropertyValue< double >( const rttr::detail::property_wrapper_base* metaProperty, const rttr::instance& refObject );
	friend void					SetPropertyValue< double >( const rttr::detail::property_wrapper_base* metaProperty, rttr::instance& refObject, double newValue );

	friend bool					GetPropertyValue< bool >( const rttr::detail::property_wrapper_base* metaProperty, const rttr::instance& refObject );
	friend void					SetPropertyValue< bool >( const rttr::detail::property_wrapper_base* metaProperty, rttr::instance& refObject, bool newValue );

	friend std::string 			GetPropertyValue< std::string >( const rttr::detail::property_wrapper_base* metaProperty, const rttr::instance& refObject );
	friend void					SetPropertyValue< std::string >( const rttr::detail::property_wrapper_base* metaProperty, rttr::instance& refObject, std::string newValue );

	friend std::wstring 		GetPropertyValue< std::wstring >( const rttr::detail::property_wrapper_base* metaProperty, const rttr::instance& refObject );
	friend void					SetPropertyValue< std::wstring >( const rttr::detail::property_wrapper_base* metaProperty, rttr::instance& refObject, std::wstring newValue );

	static rttr::property									MakeProperty		( const rttr::detail::property_wrapper_base* wrapper );
	static const rttr::detail::property_wrapper_base*		GetWrapperBase		( rttr::property prop );
};


// ================================ //
//
template< typename PropertyType >
PropertyType			InvalidValue				()
{
	return std::numeric_limits< PropertyType >::max();
}

// ================================ //
//
template<>
std::string				InvalidValue				() { return ""; }

template<>
std::wstring			InvalidValue				() { return L""; }



// ================================ //
//
template< typename PropertyType >
PropertyType	GetPropertyValue			( const rttr::detail::property_wrapper_base* metaProperty, const rttr::instance& refObject )
{
	if( refObject.is_valid() )
	{
		rttr::property prop = RTTRPropertyRapist::MakeProperty( metaProperty );

		auto value = prop.get_value( refObject );
		return value.get_value< PropertyType >();
	}

	return InvalidValue< PropertyType >();
}

// ================================ //
//
template< typename PropertyType >
void		SetPropertyValue				( const rttr::detail::property_wrapper_base* metaProperty, rttr::instance& refObject, PropertyType newValue )
{
	if( refObject.is_valid() )
	{
		rttr::property prop = RTTRPropertyRapist::MakeProperty( metaProperty );
		prop.set_value( refObject, newValue );
	}
}

}
}	// sw

