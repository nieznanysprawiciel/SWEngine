#include "PropertyWrapper.h"



namespace EditorPlugin
{



/**@brief */
template< typename PropertyType >
PropertyType	GetXMFloat( const rttr::detail::property_wrapper_base* metaProperty, System::IntPtr refObject )
{
	rttr::property prop = RTTRPropertyRapist::MakeProperty( metaProperty );

	if( prop.get_declaring_type().is_derived_from< EngineObject >() )
	{
		auto value = prop.get_value( *static_cast< EngineObject* >( refObject.ToPointer() ) );
		return value.get_value< PropertyType >();
	}
	else if( prop.get_declaring_type().is_derived_from< DirectX::XMFLOAT2 >() )
	{
		auto value = prop.get_value( static_cast< DirectX::XMFLOAT2* >( refObject.ToPointer() ) );
		return value.get_value< PropertyType >();
	}
	else if( prop.get_declaring_type().is_derived_from< DirectX::XMFLOAT3 >() )
	{
		auto value = prop.get_value( static_cast< DirectX::XMFLOAT3* >( refObject.ToPointer() ) );
		return value.get_value< PropertyType >();
	}
	else if( prop.get_declaring_type().is_derived_from< DirectX::XMFLOAT4 >() )
	{
		auto value = prop.get_value( static_cast< DirectX::XMFLOAT4* >( refObject.ToPointer() ) );
		return value.get_value< PropertyType >();
	}
	else
		assert( false );	return PropertyType();
}

/**@brief */
template< typename PropertyType >
void		SetXMFloat( const rttr::detail::property_wrapper_base* metaProperty, System::IntPtr refObject, PropertyType newValue )
{
	rttr::property prop = RTTRPropertyRapist::MakeProperty( metaProperty );

	if( prop.get_declaring_type().is_derived_from< EngineObject >() )
	{
		prop.set_value( *static_cast< EngineObject* >( refObject.ToPointer() ), newValue );
	}
	else if( prop.get_declaring_type().is_derived_from< DirectX::XMFLOAT2 >() )
	{
		prop.set_value( static_cast< DirectX::XMFLOAT2* >( refObject.ToPointer() ), newValue );
	}
	else if( prop.get_declaring_type().is_derived_from< DirectX::XMFLOAT3 >() )
	{
		prop.set_value( static_cast< DirectX::XMFLOAT3* >( refObject.ToPointer() ), newValue );
	}
	else if( prop.get_declaring_type().is_derived_from< DirectX::XMFLOAT4 >() )
	{
		prop.set_value( static_cast< DirectX::XMFLOAT4* >( refObject.ToPointer() ), newValue );
	}
}

//====================================================================================//
//			PropertyWrapper implementation	
//====================================================================================//


/**@brief */
void			PropertyWrapper::ResetActor		( System::IntPtr objectPtr )
{
	m_actorPtr = static_cast< EngineObject* >( objectPtr.ToPointer() );
}



/**************************************************************************************/
//====================================================================================//
//				DirectXMath types	
//====================================================================================//


//====================================================================================//
//				Float2PropertyWrapper
//====================================================================================//

/**@brief */
float		Float2PropertyWrapper::GetValueX	( System::IntPtr refObject )
{
	auto value = GetXMFloat< DirectX::XMFLOAT2 >( m_metaProperty, refObject );
	return value.x;
}

/**@brief */
void		Float2PropertyWrapper::SetValueX	( System::IntPtr refObject, float newValue )
{
	auto value = GetXMFloat< DirectX::XMFLOAT2 >( m_metaProperty, refObject );
	value.x = newValue;
	SetXMFloat( m_metaProperty, refObject, value );
}

/**@brief */
float		Float2PropertyWrapper::GetValueY	( System::IntPtr refObject )
{
	auto value = GetXMFloat< DirectX::XMFLOAT2 >( m_metaProperty, refObject );
	return value.y;
}

/**@brief */
void		Float2PropertyWrapper::SetValueY	( System::IntPtr refObject, float newValue )
{
	auto value = GetXMFloat< DirectX::XMFLOAT2 >( m_metaProperty, refObject );
	value.y = newValue;
	SetXMFloat( m_metaProperty, refObject, value );
}

//====================================================================================//
//				Float3PropertyWrapper
//====================================================================================//
//


/**@brief */
float		Float3PropertyWrapper::GetValueX	( System::IntPtr refObject )
{
	auto value = GetXMFloat< DirectX::XMFLOAT3 >( m_metaProperty, refObject );
	return value.x;
}

/**@brief */
void		Float3PropertyWrapper::SetValueX	( System::IntPtr refObject, float newValue )
{
	auto value = GetXMFloat< DirectX::XMFLOAT3 >( m_metaProperty, refObject );
	value.x = newValue;
	SetXMFloat( m_metaProperty, refObject, value );
}

/**@brief */
float		Float3PropertyWrapper::GetValueY	( System::IntPtr refObject )
{
	auto value = GetXMFloat< DirectX::XMFLOAT3 >( m_metaProperty, refObject );
	return value.y;
}

/**@brief */
void		Float3PropertyWrapper::SetValueY	( System::IntPtr refObject, float newValue )
{
	auto value = GetXMFloat< DirectX::XMFLOAT3 >( m_metaProperty, refObject );
	value.y = newValue;
	SetXMFloat( m_metaProperty, refObject, value );
}

/**@brief */
float		Float3PropertyWrapper::GetValueZ	( System::IntPtr refObject )
{
	auto value = GetXMFloat< DirectX::XMFLOAT3 >( m_metaProperty, refObject );
	return value.z;
}

/**@brief */
void		Float3PropertyWrapper::SetValueZ	( System::IntPtr refObject, float newValue )
{
	auto value = GetXMFloat< DirectX::XMFLOAT3 >( m_metaProperty, refObject );
	value.z = newValue;
	SetXMFloat( m_metaProperty, refObject, value );
}


//====================================================================================//
//				Float4PropertyWrapper	
//====================================================================================//

/**@brief */
float		Float4PropertyWrapper::GetValueX	( System::IntPtr refObject )
{
	auto value = GetXMFloat< DirectX::XMFLOAT4 >( m_metaProperty, refObject );
	return value.x;
}

/**@brief */
void		Float4PropertyWrapper::SetValueX	( System::IntPtr refObject, float newValue )
{
	auto value = GetXMFloat< DirectX::XMFLOAT4 >( m_metaProperty, refObject );
	value.x = newValue;
	SetXMFloat( m_metaProperty, refObject, value );
}

/**@brief */
float		Float4PropertyWrapper::GetValueY	( System::IntPtr refObject )
{
	auto value = GetXMFloat< DirectX::XMFLOAT4 >( m_metaProperty, refObject );
	return value.y;
}

/**@brief */
void		Float4PropertyWrapper::SetValueY	( System::IntPtr refObject, float newValue )
{
	auto value = GetXMFloat< DirectX::XMFLOAT4 >( m_metaProperty, refObject );
	value.y = newValue;
	SetXMFloat( m_metaProperty, refObject, value );
}

/**@brief */
float		Float4PropertyWrapper::GetValueZ	( System::IntPtr refObject )
{
	auto value = GetXMFloat< DirectX::XMFLOAT4 >( m_metaProperty, refObject );
	return value.z;
}

/**@brief */
void		Float4PropertyWrapper::SetValueZ	( System::IntPtr refObject, float newValue )
{
	auto value = GetXMFloat< DirectX::XMFLOAT4 >( m_metaProperty, refObject );
	value.z = newValue;
	SetXMFloat( m_metaProperty, refObject, value );
}

/**@brief */
float		Float4PropertyWrapper::GetValueW	( System::IntPtr refObject )
{
	auto value = GetXMFloat< DirectX::XMFLOAT4 >( m_metaProperty, refObject );
	return value.w;
}

/**@brief */
void		Float4PropertyWrapper::SetValueW	( System::IntPtr refObject, float newValue )
{
	auto value = GetXMFloat< DirectX::XMFLOAT4 >( m_metaProperty, refObject );
	value.w = newValue;
	SetXMFloat( m_metaProperty, refObject, value );
}

//====================================================================================//
//				
//====================================================================================//


/**@brief */
int IntPropertyWrapper::GetValue( System::IntPtr refObject )
{
	auto value = GetXMFloat< int >( m_metaProperty, refObject );
	return value;
}

/**@brief */
void IntPropertyWrapper::SetValue( System::IntPtr refObject, int newValue )
{
	SetXMFloat< int >( m_metaProperty, refObject, newValue );
}


/**@brief */
bool BoolPropertyWrapper::GetValue( System::IntPtr refObject )
{
	auto value = GetXMFloat< bool >( m_metaProperty, refObject );
	return value;
}

/**@brief */
void BoolPropertyWrapper::SetValue( System::IntPtr refObject, bool newValue )
{
	SetXMFloat< bool >( m_metaProperty, refObject, newValue );
}


/**@brief */
float FloatPropertyWrapper::GetValue( System::IntPtr refObject )
{
	auto value = GetXMFloat< float >( m_metaProperty, refObject );
	return value;
}

/**@brief */
void FloatPropertyWrapper::SetValue( System::IntPtr refObject, float newValue )
{
	SetXMFloat< float >( m_metaProperty, refObject, newValue );
}

/**@brief */
double DoublePropertyWrapper::GetValue( System::IntPtr refObject )
{
	auto value = GetXMFloat< double >( m_metaProperty, refObject );
	return value;
}

/**@brief */
void DoublePropertyWrapper::SetValue( System::IntPtr refObject, double newValue )
{
	SetXMFloat< double >( m_metaProperty, refObject, newValue );
}

//====================================================================================//
//			ObjectPropertyWrapper	
//====================================================================================//

/**@brief */
void			CategoryPropertyWrapper::ResetActor		( System::IntPtr objectPtr )
{
	m_actorPtr = static_cast< EngineObject* >( objectPtr.ToPointer() );

	for each( auto property in m_properties )
		property->ResetActor( objectPtr );
}

/**@brief */
void CategoryPropertyWrapper::BuildHierarchy( rttr::type classType )
{
	classType = classType.get_raw_type();
	auto properties = classType.get_properties();
	Dictionary< System::String^, CategoryPropertyWrapper^ >  categories;

	for( auto& prop : properties )
	{
		auto categoryNameMeta = prop.get_metadata( MetaDataType::Category );
		if( categoryNameMeta.is_valid() && categoryNameMeta.is_type< std::string >() )
		{
			const std::string& categoryNameStdStr = categoryNameMeta.get_value< std::string >();
			System::String^ categoryNameStr = gcnew System::String( categoryNameStdStr.c_str() );
			if( !categories.ContainsKey( categoryNameStr ) )
				categories[ categoryNameStr ] = gcnew CategoryPropertyWrapper( categoryNameStdStr.c_str() );

			categories[ categoryNameStr ]->Properties->Add( BuildProperty( prop ) );
		}
		else
		{
			if( !categories.ContainsKey( "Other" ) )
				categories[ "Other" ] = gcnew CategoryPropertyWrapper( "Other" );

			categories[ "Other" ]->Properties->Add( BuildProperty( prop ) );
		}	
	}

	for each ( auto var in categories )
		m_properties->Add( var.Value );
}

/**@brief */
PropertyWrapper^ CategoryPropertyWrapper::BuildProperty( rttr::property property )
{
	auto propertyType = property.get_type();
	if( propertyType == rttr::type::get< int >() )
	{
		return gcnew IntPropertyWrapper( property );
	}
	else if( propertyType == rttr::type::get< float >() )
	{
		return gcnew FloatPropertyWrapper( property );
	}
	else if( propertyType == rttr::type::get< bool >() )
	{
		return gcnew BoolPropertyWrapper( property );
	}
	else if( propertyType == rttr::type::get< double >() )
	{
		return gcnew DoublePropertyWrapper( property );
	}
	else if( propertyType == rttr::type::get< DirectX::XMFLOAT2* >()
			 || propertyType == rttr::type::get< DirectX::XMFLOAT3* >() 
			 || propertyType == rttr::type::get< DirectX::XMFLOAT4* >() )
	{
		auto propertyWrapper = gcnew XMFloatPropertyWrapper( property );
		propertyWrapper->BuildHierarchy();
		return propertyWrapper;
	}
	else if( propertyType.is_derived_from< EngineObject >() )
	{
		auto propertyWrapper = gcnew ObjectPropertyWrapper( property );
		propertyWrapper->BuildHierarchy();
		return propertyWrapper;
	}
	else
		throw gcnew System::NotImplementedException();
}

/**@brief Zbuduj hierarchiê metadanych z podanego obiektu.*/
void ObjectPropertyWrapper::BuildHierarchy()
{
	BuildHierarchy( RTTRPropertyRapist::MakeProperty( m_metaProperty ).get_type() );
}

/**@brief Buduje hierarchiê dla typów DirectX::XMFLOAT...

Funkcja nie grupuje Property w kategorie w przeciwieñstwie do
@ref CategoryPropertyWrapper::BuildHierarchy.
*/
void XMFloatPropertyWrapper::BuildHierarchy( rttr::type classType )
{
	classType = classType.get_raw_type();
	auto properties = classType.get_properties();

	for( auto& prop : properties )
	{
		Properties->Add( BuildProperty( prop ) );
	}
}

/**@brief Zbuduj hierarchiê metadanych z podanego obiektu.*/
void XMFloatPropertyWrapper::BuildHierarchy()
{
	BuildHierarchy( RTTRPropertyRapist::MakeProperty( m_metaProperty ).get_type() );
}

/**@brief */
void	XMFloatPropertyWrapper::ResetActor		( System::IntPtr objectPtr )
{
	auto prop = RTTRPropertyRapist::MakeProperty( m_metaProperty );

	if( m_type == PropertyType::PropertyFloat2 )
	{
		auto value = prop.get_value( *static_cast< EngineObject* >( objectPtr.ToPointer() ) );
		auto xmfloat = value.get_value< DirectX::XMFLOAT2* >();

		for each( auto property in Properties )
			property->ResetActor( System::IntPtr( (void*)xmfloat ) );
	}
	else if( m_type == PropertyType::PropertyFloat3 )
	{
		auto value = prop.get_value( *static_cast< EngineObject* >( objectPtr.ToPointer() ) );
		auto xmfloat = value.get_value< DirectX::XMFLOAT3* >();

		for each( auto property in Properties )
			property->ResetActor( System::IntPtr( (void*)xmfloat ) );
	}
	else if( m_type == PropertyType::PropertyFloat4 )
	{
		auto value = prop.get_value( *static_cast< EngineObject* >( objectPtr.ToPointer() ) );
		auto xmfloat = value.get_value< DirectX::XMFLOAT4* >();

		for each( auto property in Properties )
			property->ResetActor( System::IntPtr( (void*)xmfloat ) );
	}
	else
		assert( false );
	
}

}
