/**
@file HierarchicalPropertyWrapper.cpp
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/
#include "EngineEditor/PropertyWrapperRTTR/stdafx.h"
#include "HierarchicalPropertyWrapper.h"


#include "EngineEditor/PropertyWrapperRTTR/Properties/Basic/BasicTypesWrappers.h"
#include "EngineEditor/PropertyWrapperRTTR/Properties/Basic/EnumPropertyWrapper.h"
#include "EngineEditor/PropertyWrapperRTTR/Properties/Basic/StringPropertyWrapper.h"

#include "EngineEditor/PropertyWrapperRTTR/Properties/Array/ArrayPropertyWrapper.h"
#include "EngineEditor/PropertyWrapperRTTR/Properties/Array/ArrayElementPropertyWrapper.h"

#include "EngineEditor/PropertyWrapperRTTR/Properties/Complex/CategoryWrapper.h"
#include "EngineEditor/PropertyWrapperRTTR/Properties/Complex/ObjectPropertyWrapper.h"
#include "EngineEditor/PropertyWrapperRTTR/Properties/XMFloatPropertyWrapper.h"

#include "swCommonLib/Common/Properties/Properties.h"

// Remove in future and create proper mechanism
#include "EngineEditor/PropertyWrapperRTTR/Properties/ResourcePropertyWrapper.h"
#include "swGraphicAPI/Resources/ResourceObject.h"


namespace sw {
namespace EditorPlugin
{



// ================================ //
//
void					HierarchicalPropertyWrapper::RebuildProperty	( rttr::variant& parent, BuildContext& context )
{
	auto prevValue = GetGenericValue();
	rttr::variant prevVariant = prevValue ? *prevValue : rttr::variant();
	rttr::variant thisVariant = RecomputeObject( parent );
	
	TypeID prevType = prevVariant.get_type();
	TypeID thisType = thisVariant.get_type();

	SetGenericValue( thisVariant );

	if( prevType != thisType )
	{
		// Recreate all properties if type changed. Note that we could check if types aren't derived
		// from each other and save part of propeties.
		m_properties->Clear();
		BuildHierarchy( thisType, context );
	}
	else
	{
		for each( auto prop in m_properties )
		{
			prop->RebuildProperty( thisVariant, context );
		}
	}
}

// ================================ //
//
void					HierarchicalPropertyWrapper::BuildHierarchy		( rttr::type classType, BuildContext& context )
{
	classType = classType.get_raw_type();
	auto properties = classType.get_properties();

	for( auto& prop : properties )
	{
		Properties->Add( BuildProperty( this, prop, context ) );
	}
}


/**@brief Zbuduj hierarchiê metadanych z podanego obiektu.*/
void					HierarchicalPropertyWrapper::BuildHierarchy		( BuildContext& context )
{
	//rttr::instance parent = m_parent->GetWrappedObject();
	rttr::variant parentVariant;
	rttr::instance parent = parentVariant;
	BuildHierarchy( parent, context );
}

// ================================ //
//
void					HierarchicalPropertyWrapper::BuildHierarchy		( rttr::instance& parent, BuildContext& context )
{
	rttr::variant thisVariant = RecomputeObject( parent );
	rttr::instance thisInstance = rttr::instance( thisVariant );

	if( !::Properties::IsNullptr( thisInstance ) )
	{
		rttr::type realType = ::Properties::GetRealWrappedType( thisInstance );
		
		SetGenericValue( thisVariant );
		BuildHierarchy( realType, context );
	}
}

// ================================ //
//
PropertyWrapper^		HierarchicalPropertyWrapper::BuildProperty		( HierarchicalPropertyWrapper^ parent, rttr::property property, BuildContext& context )
{
	auto propertyType = property.get_type();

	if( propertyType.is_wrapper() )
		propertyType = propertyType.get_wrapped_type();

	if( propertyType.is_arithmetic() )
		return TryBuildArithmeticProperty( parent, property, context );
	else if( property.is_enumeration() )
	{
		return gcnew EnumPropertyWrapper( parent, property );
	}
	else if( property.is_array() )
	{
		auto propertyWrapper = gcnew ArrayPropertyWrapper( parent, property );
		propertyWrapper->RebuildProperty( *parent->GetGenericValue(), context );
		return propertyWrapper;
	}
	else if( propertyType == rttr::type::get< std::string >() )
	{
		return gcnew StringPropertyWrapper( parent, property );
	}
	else if( propertyType == rttr::type::get< std::wstring >() )
	{
		return gcnew WStringPropertyWrapper( parent, property );
	}
	else if( propertyType == rttr::type::get< DirectX::XMFLOAT2* >()
			 || propertyType == rttr::type::get< DirectX::XMFLOAT3* >()
			 || propertyType == rttr::type::get< DirectX::XMFLOAT4* >() )
	{
		auto propertyWrapper = gcnew XMFloatPropertyWrapper( parent, property );
		propertyWrapper->RebuildProperty( *parent->GetGenericValue(), context );
		return propertyWrapper;
	}
	else if( propertyType.is_derived_from< ResourceObject >() )
	{
		// Najpierw sprawdzamy czy dziedziczy po ResourceObject dopiero potem
		// EngineObject. Dziêki temu mo¿emy oddzieliæ zasoby od innych w³aœciowoœci
		// i obs³u¿yæ je inaczej.
		auto propertyWrapper = gcnew ResourcePropertyWrapper( parent, property );
		propertyWrapper->RebuildProperty( *parent->GetGenericValue(), context );
		return propertyWrapper;
	}
	else if( propertyType.is_derived_from< EngineObject >()
			 || propertyType.is_pointer() )
	{
		auto propertyWrapper = gcnew ObjectPropertyWrapper( parent, property );
		propertyWrapper->RebuildProperty( *parent->GetGenericValue(), context );
		return propertyWrapper;
	}
	else
	{
		throw gcnew System::ArgumentException( gcnew System::String( "Property type: [" )
											   + gcnew System::String( property.get_type().get_name().to_string().c_str() )
											   + gcnew System::String( "] is not supported" ),
											   gcnew System::String( "property" ) );
		return nullptr;
	}
}

// ================================ //
//
PropertyWrapper^		HierarchicalPropertyWrapper::TryBuildArithmeticProperty		( HierarchicalPropertyWrapper^ parent, rttr::property property, BuildContext& context )
{
	auto propertyType = property.get_type();

	PropertyWrapper^ propertWrapper = nullptr;

	if( propertyType == rttr::type::get< int >() ||
		propertyType == rttr::type::get< int32 >() ||
		propertyType == rttr::type::get< int16 >() ||
		propertyType == rttr::type::get< int8 >() )
	{
		propertWrapper = gcnew IntPropertyWrapper( parent, property );
	}
	if( propertyType == rttr::type::get< uint32 >() ||
		propertyType == rttr::type::get< unsigned int >() ||
		propertyType == rttr::type::get< uint16 >() ||
		propertyType == rttr::type::get< uint8 >() )
	{
		propertWrapper =  gcnew UIntPropertyWrapper( parent, property );
	}
	else if( propertyType == rttr::type::get< float >() )
	{
		propertWrapper =  gcnew FloatPropertyWrapper( parent, property );
	}
	else if( propertyType == rttr::type::get< bool >() )
	{
		propertWrapper =  gcnew BoolPropertyWrapper( parent, property );
	}
	else if( propertyType == rttr::type::get< double >() )
	{
		propertWrapper =  gcnew DoublePropertyWrapper( parent, property );
	}
	else
		throw gcnew System::ArgumentException( gcnew System::String( "Property type: [" )
												+ gcnew System::String( property.get_name().to_string().c_str() )
												+ gcnew System::String( "] is not supported" ),
												gcnew System::String( "property" ) );

	propertWrapper->RebuildProperty( *parent->GetGenericValue(), context );
	return propertWrapper;
}

// ================================ //
//
void				HierarchicalPropertyWrapper::AddPropertyChild		( PropertyWrapper^ child )
{
	m_properties->Add( child );
}

// ================================ //
//
rttr::variant		HierarchicalPropertyWrapper::RecomputeObject		( rttr::variant& parent )
{
	return RecomputeObject( rttr::instance( parent ) );
}

// ================================ //
//
rttr::variant		HierarchicalPropertyWrapper::RecomputeObject		( rttr::instance& parent )
{
	rttr::variant thisVariant;

	if( m_metaProperty )
	{
		auto property = RTTRPropertyRapist::MakeProperty( m_metaProperty );

		rttr::instance realParentInstance = parent.get_type().get_raw_type().is_wrapper() ? parent.get_wrapped_instance() : parent;

		thisVariant = property.get_value( realParentInstance );
	}

	return thisVariant.is_valid() ? thisVariant : *GetGenericValue();
}

}
}



