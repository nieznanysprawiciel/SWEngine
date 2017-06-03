#pragma once

#include "PropertyHelper.h"
#include "swCommonLib/Common/EngineObject.h"
#include "swGraphicAPI/Resources/ResourceObject.h"


#include <memory>

class EngineObject;

namespace EditorPlugin
{
using namespace System::Collections::Generic;

	public ref class PropertyWrapper
	{
	protected:

		System::String^			m_name;
		PropertyType			m_type;
		bool					m_expandProperty;

		void*					m_actorPtr;		///< WskaŸnik u¿ywany do pobierania parametrów obiektu klasy.

	protected:
		
		const rttr::detail::property_wrapper_base*			m_metaProperty;

	public:
		PropertyWrapper( void* parent, PropertyType type, rttr::property prop, const char* name )
			:	m_type( type )
			,	m_actorPtr( parent )
			,	m_expandProperty( true )
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
			bool				get	()				{ return m_expandProperty; }
			void				set	( bool value)	{ m_expandProperty = value; }
		}
	};


//====================================================================================//
//			Classes derived from PropertyWrapper	
//====================================================================================//


	/**@brief Property typy in.*/
	public ref class IntPropertyWrapper : PropertyWrapper
	{
	public:
		IntPropertyWrapper( void* parent, rttr::property prop )
			: PropertyWrapper( parent, PropertyType::PropertyInt, prop, prop.get_name().to_string().c_str() )
		{}


		property int		Value
		{
			int			get ()
			{
				if( m_actorPtr )
					return GetValue( m_actorPtr );
				return std::numeric_limits< int >::max();
			}

			void		set	( int value )
			{
				if( m_actorPtr )
					SetValue( m_actorPtr, value );
			}
		}

	private:
		int						GetValue( void* refObject );
		void					SetValue( void* refObject, int newValue );
	};

	/**@brief Property typy unsinged int.*/
	public ref class UIntPropertyWrapper : PropertyWrapper
	{
	public:
		UIntPropertyWrapper( void* parent, rttr::property prop )
			: PropertyWrapper( parent, PropertyType::PropertyUInt, prop, prop.get_name().to_string().c_str() )
		{}


		property uint32		Value
		{
			uint32		get ()
			{
				if( m_actorPtr )
					return GetValue( m_actorPtr );
				return std::numeric_limits< uint32 >::max();
			}

			void		set	( uint32 value )
			{
				if( m_actorPtr )
					SetValue( m_actorPtr, value );
			}
		}

	private:
		uint32					GetValue( void* refObject );
		void					SetValue( void* refObject, uint32 newValue );
	};

	/**@brief Property typu bool.*/
	public ref class BoolPropertyWrapper : PropertyWrapper
	{
	public:
		BoolPropertyWrapper( void* parent, rttr::property prop )
			: PropertyWrapper( parent, PropertyType::PropertyBool, prop, prop.get_name().to_string().c_str() )
		{}

		property bool		Value
		{
			bool		get ()
			{
				if( m_actorPtr )
					return GetValue( m_actorPtr );
				return std::numeric_limits< bool >::max();
			}

			void		set ( bool value )
			{
				if( m_actorPtr )
					SetValue( m_actorPtr, value );
			}
		}

	private:
		bool		GetValue( void* refObject );
		void		SetValue( void* refObject, bool newValue );
	};

	/**@brief Property typu float.*/
	public ref class FloatPropertyWrapper : PropertyWrapper
	{
	public:
		FloatPropertyWrapper( void* parent, rttr::property prop )
			: PropertyWrapper( parent, PropertyType::PropertyFloat, prop, prop.get_name().to_string().c_str() )
		{}

		property float		Value
		{
			float		get ()
			{
				if( m_actorPtr )
					return GetValue( m_actorPtr );
				return std::numeric_limits< float >::max();
			}

			void		set ( float value )
			{
				if( m_actorPtr )
					SetValue( m_actorPtr, value );
			}
		}

	private:
		float		GetValue( void* refObject );
		void		SetValue( void* refObject, float newValue );
	};

	/**@brief Property typu double.*/
	public ref class DoublePropertyWrapper : PropertyWrapper
	{
	public:
		DoublePropertyWrapper( void* parent, rttr::property prop )
			: PropertyWrapper( parent, PropertyType::PropertyDouble, prop, prop.get_name().to_string().c_str() )
		{}

	public:
		property double		Value
		{
			double		get	()
			{
				if( m_actorPtr )
					return GetValue( m_actorPtr );
				return std::numeric_limits< double >::max();
			}

			void		set	( double value )
			{
				if( m_actorPtr )
					SetValue( m_actorPtr, value );
			}
		}

	private:
		double		GetValue( void* refObject );
		void		SetValue( void* refObject, double newValue );
	};

	/**@brief Property typu std::string.*/
	public ref class StringPropertyWrapper : PropertyWrapper
	{
	public:
		StringPropertyWrapper( void* parent, rttr::property prop )
			: PropertyWrapper( parent, PropertyType::PropertyString, prop, prop.get_name().to_string().c_str() )
		{}

	public:
		property System::String^		Value
		{
			System::String^		get	()
			{
				if( m_actorPtr )
					return GetValue( m_actorPtr );
				return gcnew System::String( "" );
			}

			void				set	( System::String^ value )
			{
				if( m_actorPtr )
					SetValue( m_actorPtr, value );
			}
		}

	private:
		System::String^		GetValue( void* refObject );
		void				SetValue( void* refObject, System::String^ newValue );
	};

	/**@brief Property typu std::wstring.*/
	public ref class WStringPropertyWrapper : PropertyWrapper
	{
	public:
		WStringPropertyWrapper( void* parent, rttr::property prop )
			: PropertyWrapper( parent, PropertyType::PropertyWString, prop, prop.get_name().to_string().c_str() )
		{}

	public:
		property System::String^		Value
		{
			System::String^		get	()
			{
				if( m_actorPtr )
					return GetValue( m_actorPtr );
				return gcnew System::String( "" );
			}

			void				set	( System::String^ value )
			{
				if( m_actorPtr )
					SetValue( m_actorPtr, value );
			}
		}

	private:
		System::String^		GetValue( void* refObject );
		void				SetValue( void* refObject, System::String^ newValue );
	};

	/**@brief Property enum.*/
	public ref class EnumPropertyWrapper : PropertyWrapper
	{
	private:
	protected:

		List< System::String^ >^		m_enumStrings;

	public:
		EnumPropertyWrapper( void* parent, rttr::property prop );

	public:
		property System::String^		Value
		{
			System::String^		get	()
			{
				if( m_actorPtr )
					return GetValue( m_actorPtr );
				return gcnew System::String( "" );
			}

			void				set	( System::String^ value )
			{
				if( m_actorPtr )
					SetValue( m_actorPtr, value );
			}
		}

		property List< System::String^ >^	EnumStrings
		{
			List< System::String^ >^		get()
			{
				return m_enumStrings;
			}
		}

	private:
		System::String^		GetValue( void* refObject );
		void				SetValue( void* refObject, System::String^ newValue );
	};

//====================================================================================//
//			Generic object wrapper	
//====================================================================================//

	/**@brief Klasa bêd¹ca zbiorem Property nale¿¹cych do jednej kategorii.*/
	public ref class CategoryPropertyWrapper : PropertyWrapper
	{
	private:

		List< PropertyWrapper^ >^			m_properties;

	protected:

		CategoryPropertyWrapper( void* parent, PropertyType type, rttr::property prop, const char* name )
			: PropertyWrapper( parent, type, prop, name )
		{
			m_properties = gcnew List< PropertyWrapper^ >();
		}

	public:
		CategoryPropertyWrapper( void* parent, const char* name )
			: PropertyWrapper( parent, PropertyType::PropertyCategory, RTTRPropertyRapist::MakeProperty( nullptr ), name )
		{
			m_properties = gcnew List< PropertyWrapper^ >();
		}

		virtual void			BuildHierarchy	( void* objectPtr, rttr::type classType );

	protected:
		PropertyWrapper^		BuildProperty	( void* parent, rttr::property property );

	public:
		property List< PropertyWrapper^ >^	Properties
		{
			List< PropertyWrapper^ >^		get() { return m_properties; }
		}
	};


	/**@brief Property dla typów, które nie grupuj¹ swoich w³aœciwoœci w podkategorie
	tylko wyœwietlaj¹ je bezpoœrednio.*/
	public ref class CategoryLessPropertyWrapper : CategoryPropertyWrapper
	{
	private:
	protected:
		CategoryLessPropertyWrapper( void* parent, PropertyType type, rttr::property prop, const char* name )
			: CategoryPropertyWrapper( parent, type, prop, name )
		{}

	public:
		CategoryLessPropertyWrapper( void* parent, const char* name )
			: CategoryPropertyWrapper( parent, name )
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
