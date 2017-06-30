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

// Remove in future and create proper mechanism
#include "EngineEditor/PropertyWrapperRTTR/Properties/ResourcePropertyWrapper.h"
#include "swGraphicAPI/Resources/ResourceObject.h"


namespace sw {
namespace EditorPlugin
{


// ================================ //
//
HierarchicalPropertyWrapper::~HierarchicalPropertyWrapper()
{
	delete m_wrappedObject;
}


// ================================ //
//
void					HierarchicalPropertyWrapper::BuildHierarchy		( const rttr::instance& parent, rttr::type classType, BuildContext& context )
{
	classType = classType.get_raw_type();
	auto properties = classType.get_properties();

	for( auto& prop : properties )
	{
		Properties->Add( BuildProperty( this, prop, context ) );
	}
}


void* VoidMove( void* obj ) { return obj; }

/**@brief Zbuduj hierarchiê metadanych z podanego obiektu.*/
void					HierarchicalPropertyWrapper::BuildHierarchy		( BuildContext& context )
{
	auto property = RTTRPropertyRapist::MakeProperty( m_metaProperty );

	rttr::instance parent = m_parent->GetWrappedObject();
    rttr::instance parentInst = parent.get_type().get_raw_type().is_wrapper() ? parent.get_wrapped_instance() : parent;

	rttr::instance thisWrappedObj = property.get_value( parentInst );

	if( thisWrappedObj.is_valid() )
	{
		rttr::type realType = thisWrappedObj.get_derived_type();
		
		SetWrappedObject( thisWrappedObj );
		BuildHierarchy( thisWrappedObj, realType, context );
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
		return TryBuildArithmeticProperty( parent, property );
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
		propertyWrapper->BuildHierarchy( context );
		return propertyWrapper;
	}
	else if( propertyType.is_derived_from< ResourceObject >() )
	{
		// Najpierw sprawdzamy czy dziedziczy po ResourceObject dopiero potem
		// EngineObject. Dziêki temu mo¿emy oddzieliæ zasoby od innych w³aœciowoœci
		// i obs³u¿yæ je inaczej.
		auto propertyWrapper = gcnew ResourcePropertyWrapper( parent, property );
		propertyWrapper->BuildHierarchy( context );
		return propertyWrapper;
	}
	else if( propertyType.is_derived_from< EngineObject >()
			 || propertyType.is_pointer() )
	{
		auto propertyWrapper = gcnew ObjectPropertyWrapper( parent, property );
		propertyWrapper->BuildHierarchy( context );
		return propertyWrapper;
	}
	else if( property.is_enumeration() )
	{
		return gcnew EnumPropertyWrapper( parent, property );
	}
	else if( property.is_array() )
	{
		auto propertyWrapper = gcnew ArrayPropertyWrapper( parent, property );
		propertyWrapper->BuildHierarchy( context );
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
PropertyWrapper^		HierarchicalPropertyWrapper::TryBuildArithmeticProperty		( HierarchicalPropertyWrapper^ parent, rttr::property property )
{
	auto propertyType = property.get_type();

	if( propertyType == rttr::type::get< int >() ||
		propertyType == rttr::type::get< int32 >() ||
		propertyType == rttr::type::get< int16 >() ||
		propertyType == rttr::type::get< int8 >() )
	{
		return gcnew IntPropertyWrapper( parent, property );
	}
	if( propertyType == rttr::type::get< uint32 >() ||
		propertyType == rttr::type::get< unsigned int >() ||
		propertyType == rttr::type::get< uint16 >() ||
		propertyType == rttr::type::get< uint8 >() )
	{
		return gcnew UIntPropertyWrapper( parent, property );
	}
	else if( propertyType == rttr::type::get< float >() )
	{
		return gcnew FloatPropertyWrapper( parent, property );
	}
	else if( propertyType == rttr::type::get< bool >() )
	{
		return gcnew BoolPropertyWrapper( parent, property );
	}
	else if( propertyType == rttr::type::get< double >() )
	{
		return gcnew DoublePropertyWrapper( parent, property );
	}
	else
		throw gcnew System::ArgumentException( gcnew System::String( "Property type: [" )
												+ gcnew System::String( property.get_name().to_string().c_str() )
												+ gcnew System::String( "] is not supported" ),
												gcnew System::String( "property" ) );
	return nullptr;
}

// ================================ //
//
void				HierarchicalPropertyWrapper::AddPropertyChild		( PropertyWrapper^ child )
{
	m_properties->Add( child );
}

// ================================ //
//
void				HierarchicalPropertyWrapper::SetWrappedObject		( const rttr::instance& owner )
{
	if( m_wrappedObject )
		delete m_wrappedObject;

	m_wrappedObject = new rttr::instance( owner );
}



}
}



