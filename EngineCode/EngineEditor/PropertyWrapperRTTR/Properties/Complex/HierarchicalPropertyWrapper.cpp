/**
@file HierarchicalPropertyWrapper.cpp
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/
#include "EngineEditor/PropertyWrapperRTTR/stdafx.h"
#include "HierarchicalPropertyWrapper.h"

#include "EngineEditor/PropertyWrapperRTTR/Build/PropertyBuilder.h"


#include "swCommonLib/Common/Properties/Properties.h"



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

	if( !::Properties::IsNullptr( thisVariant ) )
	{
		if( prevType != thisType || m_boundAsReference )
		{
			// Recreate all properties if type changed. Note that we could check if types aren't derived
			// from each other and save part of propeties.
			m_properties->Clear();
			BuildProperties( thisType, context );
		}
		else
		{
			RebuildProperties( thisVariant, context );
		}
	}
	else
	{
		m_properties->Clear();
	}
}

// ================================ //
//
void					HierarchicalPropertyWrapper::RebuildProperties		( rttr::variant& thisVariant, BuildContext& context )
{
	for each( auto prop in m_properties )
	{
		prop->RebuildProperty( thisVariant, context );
	}
}

// ================================ //
//
void					HierarchicalPropertyWrapper::BuildProperties		( rttr::type classType, BuildContext& context )
{
	classType = classType.is_wrapper() ? classType.get_wrapped_type() : classType;
	classType = classType.get_raw_type();
	auto properties = classType.get_properties();

	for( auto& prop : properties )
	{
		Properties->Add( PropertyBuilder::BuildProperty( this, prop, context ) );
	}
}


/**@brief Zbuduj hierarchiê metadanych z podanego obiektu.*/
void					HierarchicalPropertyWrapper::BuildHierarchy		( BuildContext& context )
{
	BuildHierarchy( rttr::instance( rttr::variant() ), context );
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
		BuildProperties( realType, context );
	}
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

	// Object can be a property or it can have no parent to reference. In second case
	// GetGenericValue will always return the same variant.
	if( m_metaProperty )
	{
		auto property = RTTRPropertyRapist::MakeProperty( m_metaProperty );

		rttr::instance realParentInstance = parent.get_type().get_raw_type().is_wrapper() ? parent.get_wrapped_instance() : parent;

		thisVariant = property.get_value( realParentInstance );

		if( thisVariant.is_valid() )
			return thisVariant;
	}
	else
	{
		auto variantPtr = GetGenericValue();
		if( variantPtr )
			return *variantPtr;
	}

	return rttr::variant();
}

}
}



