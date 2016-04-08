#pragma once

#include "Common/RTTR.h"
#include "Common/TypesDefinitions.h"
#include <DirectXMath.h>


namespace EditorPlugin
{

	public enum PropertyType
	{
		PropertyFloat,
		PropertyFloat2,
		PropertyFloat3,
		PropertyFloat4,
		PropertyInt,
		PropertyBool,
		PropertyDouble,
		PropertyList,
		PropertyCategory
	};

	template< typename PropertyType >
	PropertyType		GetXMFloat( const rttr::detail::property_wrapper_base* metaProperty, System::IntPtr refObject );

	template< typename PropertyType >
	void				SetXMFloat( const rttr::detail::property_wrapper_base* metaProperty, System::IntPtr refObject, PropertyType newValue );

	/**@brief Helper class to extract private property_wrapper_base from rttr::property.*/
	public class RTTRPropertyRapist
	{
		friend ref class PropertyWrapper;
		friend ref class FloatPropertyWrapper;
		friend ref class Float2PropertyWrapper;
		friend ref class Float3PropertyWrapper;
		friend ref class Float4PropertyWrapper;
		friend ref class IntPropertyWrapper;
		friend ref class BoolPropertyWrapper;
		friend ref class ActorClassMetaInfo;
		friend ref class CategoryPropertyWrapper;

		friend DirectX::XMFLOAT2	GetXMFloat< DirectX::XMFLOAT2 >( const rttr::detail::property_wrapper_base* metaProperty, System::IntPtr refObject );
		friend void					SetXMFloat< DirectX::XMFLOAT2 >( const rttr::detail::property_wrapper_base* metaProperty, System::IntPtr refObject, DirectX::XMFLOAT2 newValue );

		friend DirectX::XMFLOAT3	GetXMFloat< DirectX::XMFLOAT3 >( const rttr::detail::property_wrapper_base* metaProperty, System::IntPtr refObject );
		friend void					SetXMFloat< DirectX::XMFLOAT3 >( const rttr::detail::property_wrapper_base* metaProperty, System::IntPtr refObject, DirectX::XMFLOAT3 newValue );

		friend DirectX::XMFLOAT4	GetXMFloat< DirectX::XMFLOAT4 >( const rttr::detail::property_wrapper_base* metaProperty, System::IntPtr refObject );
		friend void					SetXMFloat< DirectX::XMFLOAT4 >( const rttr::detail::property_wrapper_base* metaProperty, System::IntPtr refObject, DirectX::XMFLOAT4 newValue );

		friend int					GetXMFloat< int >( const rttr::detail::property_wrapper_base* metaProperty, System::IntPtr refObject );
		friend void					SetXMFloat< int >( const rttr::detail::property_wrapper_base* metaProperty, System::IntPtr refObject, int newValue );

		friend float				GetXMFloat< float >( const rttr::detail::property_wrapper_base* metaProperty, System::IntPtr refObject );
		friend void					SetXMFloat< float >( const rttr::detail::property_wrapper_base* metaProperty, System::IntPtr refObject, float newValue );

		friend double				GetXMFloat< double >( const rttr::detail::property_wrapper_base* metaProperty, System::IntPtr refObject );
		friend void					SetXMFloat< double >( const rttr::detail::property_wrapper_base* metaProperty, System::IntPtr refObject, double newValue );

		friend bool					GetXMFloat< bool >( const rttr::detail::property_wrapper_base* metaProperty, System::IntPtr refObject );
		friend void					SetXMFloat< bool >( const rttr::detail::property_wrapper_base* metaProperty, System::IntPtr refObject, bool newValue );

		inline static rttr::property									MakeProperty		( const rttr::detail::property_wrapper_base* wrapper )
		{		return rttr::property( wrapper );		}

		inline static const rttr::detail::property_wrapper_base*		GetWrapperBase	( rttr::property prop )
		{		return prop.m_wrapper;		}
	};


}


