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



namespace sw {
namespace EditorPlugin
{

// ================================ //
//
void			HierarchicalPropertyWrapper::BuildHierarchy( void* parent, rttr::type classType )
{
	m_actorPtr = parent;

	classType = classType.get_raw_type();
	auto properties = classType.get_properties();

	for( auto& prop : properties )
	{
		Properties->Add( BuildProperty( parent, prop ) );
	}
}


void* VoidMove( void* obj ) { return obj; }

/**@brief Zbuduj hierarchiê metadanych z podanego obiektu.*/
void			HierarchicalPropertyWrapper::BuildHierarchy()
{
	auto property = RTTRPropertyRapist::MakeProperty( m_metaProperty );

	rttr::variant declaringObject( VoidMove( m_actorPtr ) );
	bool success = declaringObject.unsafe_convert_void( property.get_declaring_type_ptr() );

	assert( success );

	// Trzeba pobraæ realny type w³aœciwoœci. Mo¿e byæ tak, ¿e w³aœciwoœæ jest klas¹ bazow¹,
	// a tak my chcemy zbudowaæ hierarchiê dla klasy pochodnej.
	auto realContent = property.get_value( declaringObject );

	void* realPtr = realContent.get_value< void* >();
	rttr::type realType = realContent.get_type();

	// Obs³ugujemy type owrappowane.
	if( realContent.get_type().is_wrapper() && realPtr != nullptr )
	{
		rttr::instance realInstance( realContent );
		realPtr = realInstance.get_wrapped_instance();
		realType = realContent.get_type().get_wrapped_type();
	}

	if( realPtr != nullptr )
		BuildHierarchy( realPtr, realType );
}

// ================================ //
//
void				HierarchicalPropertyWrapper::BuildHierarchy( void* parentPtr, rttr::type classType )
{
	m_actorPtr = parentPtr;

	classType = classType.get_raw_type();
	auto properties = classType.get_properties();
	Dictionary< System::String^, CategoryWrapper^ >  categories;

	for( auto& prop : properties )
	{
		auto categoryNameMeta = prop.get_metadata( MetaDataType::Category );
		if( categoryNameMeta.is_valid() && categoryNameMeta.is_type< std::string >() )
		{
			const std::string& categoryNameStdStr = categoryNameMeta.get_value< std::string >();
			System::String^ categoryNameStr = gcnew System::String( categoryNameStdStr.c_str() );
			if( !categories.ContainsKey( categoryNameStr ) )
				categories[ categoryNameStr ] = gcnew CategoryWrapper( parentPtr, categoryNameStdStr.c_str() );

			categories[ categoryNameStr ]->Properties->Add( BuildProperty( parentPtr, prop ) );
		}
		else
		{
			if( !categories.ContainsKey( "Other" ) )
				categories[ "Other" ] = gcnew CategoryWrapper( parentPtr, "Other" );

			categories[ "Other" ]->Properties->Add( BuildProperty( parentPtr, prop ) );
		}
	}

	for each ( auto var in categories )
		AddPropertyChild( var.Value );
}

// ================================ //
//
PropertyWrapper^	HierarchicalPropertyWrapper::BuildProperty( void* parent, rttr::property property )
{
	auto propertyType = property.get_type();

	if( propertyType.is_wrapper() )
		propertyType = propertyType.get_wrapped_type();

	if( propertyType.is_arithmetic() )
	{
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
		propertyWrapper->BuildHierarchy();
		return propertyWrapper;
	}
	else if( propertyType.is_derived_from< ResourceObject >() )
	{
		// Najpierw sprawdzamy czy dziedziczy po ResourceObject dopiero potem
		// EngineObject. Dziêki temu mo¿emy oddzieliæ zasoby od innych w³aœciowoœci
		// i obs³u¿yæ je inaczej.
		auto propertyWrapper = gcnew ResourcePropertyWrapper( parent, property );
		propertyWrapper->BuildHierarchy();
		return propertyWrapper;
	}
	else if( propertyType.is_derived_from< EngineObject >()
			 || propertyType.is_pointer() )
	{
		auto propertyWrapper = gcnew ObjectPropertyWrapper( parent, property );
		propertyWrapper->BuildHierarchy();
		return propertyWrapper;
	}
	else if( property.is_enumeration() )
	{
		return gcnew EnumPropertyWrapper( parent, property );
	}
	else if( property.is_array() )
	{
		auto propertyWrapper = gcnew ArrayPropertyWrapper( parent, property );
		propertyWrapper->BuildHierarchy();
		return propertyWrapper;
	}
	else
		throw gcnew System::ArgumentException( gcnew System::String( "Property type: [" )
											   + gcnew System::String( property.get_type().get_name().to_string().c_str() )
											   + gcnew System::String( "] is not supported" ),
											   gcnew System::String( "property" ) );
}

// ================================ //
//
void				HierarchicalPropertyWrapper::AddPropertyChild		( PropertyWrapper^ child )
{
	m_properties->Add( child );
}



}
}



