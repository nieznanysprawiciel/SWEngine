#pragma warning(disable : 4561)
#include "EngineEditor/PropertyWrapperRTTR/stdafx.h"

#include "PropertyWrapper.h"
#include "swCommonLib/Common/EngineObject.h"
#include "swGraphicAPI/Resources/ResourceObject.h"

#include "PropertyHelper.h"

#include <msclr/marshal_cppstd.h>




namespace sw {
namespace EditorPlugin
{

// ================================ //
//
PropertyWrapper::~PropertyWrapper()
{
	if( m_propertyValue )
		delete m_propertyValue;
}

// ================================ //
//
PropertyType		PropertyWrapper::GetPropertyType		( rttr::property prop )
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

// ================================ //
//
void				PropertyWrapper::RebuildProperty		( rttr::variant& parent, BuildContext& context )
{
	if( m_metaProperty )
	{
		if( parent.is_valid() )
		{
			rttr::property prop = RTTRPropertyRapist::MakeProperty( m_metaProperty );
			SetGenericValue( prop.get_value( parent ) );
		}
		else
		{
			throw gcnew System::ArgumentException( gcnew System::String( "Property: [" ) + m_name + gcnew System::String( "] has nullptr parent." ) );
		}
	}
	else
	{
		throw gcnew System::ArgumentException( gcnew System::String( "Property: [" ) + m_name + gcnew System::String( "] has nullptr m_metaProperty field." ) );
	}
}

// ================================ //
//
void				PropertyWrapper::SetGenericValue		( const rttr::variant& value )
{
	if( m_propertyValue )
		delete m_propertyValue;

	m_propertyValue = new rttr::variant( value );
}

// ================================ //
//
rttr::variant*		PropertyWrapper::GetGenericValue		()
{
	return m_propertyValue;
}

}
}	// sw

