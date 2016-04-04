#pragma once

//using Float3 = System::Numerics::;

#include "Common/RTTR.h"
#include <memory>

#include <DirectXMath.h>

class IEnableProperty;

namespace EditorPlugin
{
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



	public ref class PropertyWrapper
	{
	protected:
		
		const rttr::detail::property_wrapper_base*			m_metaProperty;

	public:
		PropertyWrapper( rttr::property prop )
		{
			m_metaProperty = RTTRPropertyRapist::GetWrapperBase( prop );
		}

	};


//====================================================================================//
//			Classes derived from PropertyWrapper	
//====================================================================================//

	public ref class Float2PropertyWrapper : PropertyWrapper
	{
	public:
		Float2PropertyWrapper( rttr::property prop )
			: PropertyWrapper( prop )
		{}

		float		GetValueX		( System::IntPtr refObject );
		void		SetValueX		( System::IntPtr refObject, float newValue );

		float		GetValueY		( System::IntPtr refObject );
		void		SetValueY		( System::IntPtr refObject, float newValue );

	};

	public ref class Float3PropertyWrapper : PropertyWrapper
	{
	public:
		Float3PropertyWrapper( rttr::property prop )
			: PropertyWrapper( prop )
		{}

		float		GetValueX		( System::IntPtr refObject );
		void		SetValueX		( System::IntPtr refObject, float newValue );

		float		GetValueY		( System::IntPtr refObject );
		void		SetValueY		( System::IntPtr refObject, float newValue );

		float		GetValueZ		( System::IntPtr refObject );
		void		SetValueZ		( System::IntPtr refObject, float newValue );
	};

	public ref class Float4PropertyWrapper : PropertyWrapper
	{
	public:
		Float4PropertyWrapper( rttr::property prop )
			: PropertyWrapper( prop )
		{}


		float		GetValueX		( System::IntPtr refObject );
		void		SetValueX		( System::IntPtr refObject, float newValue );

		float		GetValueY		( System::IntPtr refObject );
		void		SetValueY		( System::IntPtr refObject, float newValue );

		float		GetValueZ		( System::IntPtr refObject );
		void		SetValueZ		( System::IntPtr refObject, float newValue );

		float		GetValueW		( System::IntPtr refObject );
		void		SetValueW		( System::IntPtr refObject, float newValue );
	};

	public ref class IntPropertyWrapper : PropertyWrapper
	{
	public:
		IntPropertyWrapper( rttr::property prop )
			: PropertyWrapper( prop )
		{}

		int						GetValue		( System::IntPtr refObject );
		void					SetValue		( System::IntPtr refObject, int newValue );
	};


	public ref class BoolPropertyWrapper : PropertyWrapper
	{
	public:
		BoolPropertyWrapper( rttr::property prop )
			: PropertyWrapper( prop )
		{}

		bool		GetValue		( System::IntPtr refObject );
		void		SetValue		( System::IntPtr refObject, bool newValue );
	};


	public ref class FloatPropertyWrapper : PropertyWrapper
	{
	public:
		FloatPropertyWrapper( rttr::property prop )
			: PropertyWrapper( prop )
		{}

		float		GetValue		( System::IntPtr refObject );
		void		SetValue		( System::IntPtr refObject, float newValue );
	};

	public ref class DoublePropertyWrapper : PropertyWrapper
	{
	public:
		DoublePropertyWrapper( rttr::property prop )
			: PropertyWrapper( prop )
		{}

		double		GetValue		( System::IntPtr refObject );
		void		SetValue		( System::IntPtr refObject, double newValue );
	};

//====================================================================================//
//			Generic object wrapper	
//====================================================================================//

	public ref class ObjectPropertyWrapper : PropertyWrapper
	{
	public:
		ObjectPropertyWrapper( rttr::property prop )
			: PropertyWrapper( prop )
		{}

	};

}
