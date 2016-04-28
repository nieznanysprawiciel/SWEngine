#pragma once

//using Float3 = System::Numerics::;

#include "PropertyHelper.h"
#include "Common/EngineObject.h"
#include "GraphicAPI/ResourceObject.h"


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

		EngineObject*			m_actorPtr;		///< WskaŸnik u¿ywany do pobierania parametrów obiektu klasy.

	protected:
		
		const rttr::detail::property_wrapper_base*			m_metaProperty;

	public:
		PropertyWrapper( PropertyType type, rttr::property prop, const char* name )
			: m_type( type )
			, m_actorPtr( nullptr )
		{
			m_metaProperty = RTTRPropertyRapist::GetWrapperBase( prop );
			m_name = gcnew System::String( name );
		}


		virtual void			ResetActor		( System::IntPtr objectPtr );
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
	};


//====================================================================================//
//			Classes derived from PropertyWrapper	
//====================================================================================//

#pragma region UnusedXMFloatProperties
		/**@brief Property typu DirectX::XMFLOAT2.
	@attention KLasa nieu¿ywana.*/
	public ref class Float2PropertyWrapper : PropertyWrapper
	{
	public:
		Float2PropertyWrapper( rttr::property prop )
			: PropertyWrapper( PropertyType::PropertyFloat2, prop, prop.get_name().c_str() )
		{}

		float		GetValueX		( EngineObject* refObject );
		void		SetValueX		( EngineObject* refObject, float newValue );

		float		GetValueY		( EngineObject* refObject );
		void		SetValueY		( EngineObject* refObject, float newValue );

	};

	/**@brief Property typu DirectX::XMFLOAT3.
	@attention KLasa nieu¿ywana.*/
	public ref class Float3PropertyWrapper : PropertyWrapper
	{
	public:
		Float3PropertyWrapper( rttr::property prop )
			: PropertyWrapper( PropertyType::PropertyFloat3, prop, prop.get_name().c_str() )
		{}

		float		GetValueX		( EngineObject* refObject );
		void		SetValueX		( EngineObject* refObject, float newValue );

		float		GetValueY		( EngineObject* refObject );
		void		SetValueY		( EngineObject* refObject, float newValue );

		float		GetValueZ		( EngineObject* refObject );
		void		SetValueZ		( EngineObject* refObject, float newValue );
	};

	/**@brief Property typu DirectX::XMFLOAT4.
	@attention KLasa nieu¿ywana.*/
	public ref class Float4PropertyWrapper : PropertyWrapper
	{
	public:
		Float4PropertyWrapper( rttr::property prop )
			: PropertyWrapper( PropertyType::PropertyFloat4, prop, prop.get_name().c_str() )
		{}


		float		GetValueX		( EngineObject* refObject );
		void		SetValueX		( EngineObject* refObject, float newValue );

		float		GetValueY		( EngineObject*refObject );
		void		SetValueY		( EngineObject* refObject, float newValue );

		float		GetValueZ		( EngineObject* refObject );
		void		SetValueZ		( EngineObject* refObject, float newValue );

		float		GetValueW		( EngineObject* refObject );
		void		SetValueW		( EngineObject* refObject, float newValue );
	};

#pragma endregion


	/**@brief Property typy in.*/
	public ref class IntPropertyWrapper : PropertyWrapper
	{
	public:
		IntPropertyWrapper( rttr::property prop )
			: PropertyWrapper( PropertyType::PropertyInt, prop, prop.get_name().c_str() )
		{}

		int						GetValue		( EngineObject* refObject );
		void					SetValue		( EngineObject* refObject, int newValue );

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
	};

	/**@brief Property typu bool.*/
	public ref class BoolPropertyWrapper : PropertyWrapper
	{
	public:
		BoolPropertyWrapper( rttr::property prop )
			: PropertyWrapper( PropertyType::PropertyBool, prop, prop.get_name().c_str() )
		{}

		bool		GetValue		( EngineObject* refObject );
		void		SetValue		( EngineObject* refObject, bool newValue );

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
	};

	/**@brief Property typu float.*/
	public ref class FloatPropertyWrapper : PropertyWrapper
	{
	public:
		FloatPropertyWrapper( rttr::property prop )
			: PropertyWrapper( PropertyType::PropertyFloat, prop, prop.get_name().c_str() )
		{}

		float		GetValue		( EngineObject* refObject );
		void		SetValue		( EngineObject* refObject, float newValue );

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
	};

	/**@brief Property typu double.*/
	public ref class DoublePropertyWrapper : PropertyWrapper
	{
	public:
		DoublePropertyWrapper( rttr::property prop )
			: PropertyWrapper( PropertyType::PropertyDouble, prop, prop.get_name().c_str() )
		{}

		double		GetValue		( EngineObject* refObject );
		void		SetValue		( EngineObject* refObject, double newValue );

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
	};

	/**@brief Property typu std::string.*/
	public ref class StringPropertyWrapper : PropertyWrapper
	{
	public:
		StringPropertyWrapper( rttr::property prop )
			: PropertyWrapper( PropertyType::PropertyString, prop, prop.get_name().c_str() )
		{}

		System::String^		GetValue		( EngineObject* refObject );
		void				SetValue		( EngineObject* refObject, System::String^ newValue );

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
	};

	/**@brief Property typu std::wstring.*/
	public ref class WStringPropertyWrapper : PropertyWrapper
	{
	public:
		WStringPropertyWrapper( rttr::property prop )
			: PropertyWrapper( PropertyType::PropertyWString, prop, prop.get_name().c_str() )
		{}

		System::String^		GetValue		( EngineObject* refObject );
		void				SetValue		( EngineObject* refObject, System::String^ newValue );

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

		CategoryPropertyWrapper( PropertyType type, rttr::property prop, const char* name )
			: PropertyWrapper( type, prop, name )
		{
			m_properties = gcnew List< PropertyWrapper^ >();
		}

	public:
		CategoryPropertyWrapper( const char* name )
			: PropertyWrapper( PropertyType::PropertyCategory, RTTRPropertyRapist::MakeProperty( nullptr ), name )
		{
			m_properties = gcnew List< PropertyWrapper^ >();
		}

		virtual void			ResetActor		( System::IntPtr objectPtr ) override;
		virtual void			BuildHierarchy	( rttr::type classType );

	protected:
		PropertyWrapper^		BuildProperty	( rttr::property property );

	public:
		property List< PropertyWrapper^ >^	Properties
		{
			List< PropertyWrapper^ >^		get() { return m_properties; }
		}
	};



	/**@brief Property dla obiektów z³o¿onych dziedzicz¹cych po EngineObject.*/
	public ref class ObjectPropertyWrapper : CategoryPropertyWrapper
	{
	private:

		List< PropertyWrapper^ >^			m_properties;

	public:
		ObjectPropertyWrapper( rttr::property prop )
			: CategoryPropertyWrapper( PropertyType::PropertyActor, prop, prop.get_name().c_str() )
		{}

		void					BuildHierarchy	();

	};

	/**@brief Property dla typów, które nie grupuj¹ swoich w³aœciwoœci w podkategorie
	tylko wyœwietlaj¹ je bezpoœrednio.*/
	public ref class CategoryLessPropertyWrapper : CategoryPropertyWrapper
	{
	private:
	protected:
		CategoryLessPropertyWrapper( PropertyType type, rttr::property prop, const char* name )
			: CategoryPropertyWrapper( type, prop, name )
		{}

	public:

		void			BuildHierarchy	( rttr::type classType ) override;
		void			BuildHierarchy	();

	public:
		virtual void	ResetActor		( System::IntPtr objectPtr ) override;
	};

	/**@brief Property typu DirectX::XMFLOAT2, XMFLOAT3, XMFLOAT4.*/
	public ref class XMFloatPropertyWrapper : CategoryLessPropertyWrapper
	{
	private:
	public:
		XMFloatPropertyWrapper( rttr::property prop )
			: CategoryLessPropertyWrapper( GetPropertyType( prop ), prop, prop.get_name().c_str() )
		{}
	};



}
