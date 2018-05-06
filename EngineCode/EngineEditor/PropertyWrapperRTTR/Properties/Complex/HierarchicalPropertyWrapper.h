#pragma once
/**
@file HierarchicalPropertyWrapper.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/

#include "EngineEditor/PropertyWrapperRTTR/Properties/Base/PropertyWrapper.h"

#include "EngineEditor/PropertyWrapperRTTR/Build/BuildContext.h"
#include "EngineEditor/PropertyWrapperRTTR/Build/IncludeRTTR.h"



namespace sw {
namespace EditorPlugin
{

using namespace System::Collections::Generic;



/**@brief Base class for hierarchical properties like classes, structures (which have sub properties).
*/
public ref class HierarchicalPropertyWrapper : PropertyWrapper
{
protected:

	List< PropertyWrapper^ >^		m_properties;

protected:

	HierarchicalPropertyWrapper( HierarchicalPropertyWrapper^ parent, PropertyType type, rttr::property prop, const char* name )
		:	PropertyWrapper( parent, type, prop, name )
	{
		m_properties = gcnew List< PropertyWrapper^ >();
	}

public:

	HierarchicalPropertyWrapper( HierarchicalPropertyWrapper^ parent, const char* name )
		:	PropertyWrapper( parent, PropertyType::PropertyActor, RTTRPropertyRapist::MakeProperty( nullptr ), name )
	{
		m_properties = gcnew List< PropertyWrapper^ >();
	}

	/**@brief Initialize property without parent (on top level hierarchy)*/
	HierarchicalPropertyWrapper( const rttr::variant& objectPtr, const char* name )
		:	PropertyWrapper( nullptr, PropertyType::PropertyActor, RTTRPropertyRapist::MakeProperty( nullptr ), name )
	{
		m_properties = gcnew List< PropertyWrapper^ >();
		SetGenericValue( objectPtr );
	}


	virtual void			RebuildProperty		( rttr::variant& parent, BuildContext& context ) override;
	virtual void			RebuildProperties	( rttr::variant& thisVariant, BuildContext& context );
	virtual void			BuildProperties		( rttr::type classType, BuildContext& context );
	void					BuildHierarchy		( rttr::instance& parent, BuildContext& context );
	void					BuildHierarchy		( BuildContext& context );

	rttr::instance			GetWrappedObject	() { return rttr::instance( *GetGenericValue() ); }

	rttr::variant			RecomputeObject		( rttr::variant& parent );
	rttr::variant			RecomputeObject		( rttr::instance& parent );

protected:

	void					AddPropertyChild			( PropertyWrapper^ child );

public:

	property List< PropertyWrapper^ >^	Properties
	{
		List< PropertyWrapper^ >^		get() { return m_properties; }
	}
};

}
}

