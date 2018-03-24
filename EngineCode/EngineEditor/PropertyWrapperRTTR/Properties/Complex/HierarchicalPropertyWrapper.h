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
private:

	List< PropertyWrapper^ >^		m_properties;
	rttr::instance*					m_wrappedObject;

protected:

	HierarchicalPropertyWrapper( HierarchicalPropertyWrapper^ parent, PropertyType type, rttr::property prop, const char* name )
		:	PropertyWrapper( parent, type, prop, name )
		,	m_wrappedObject( nullptr )
	{
		m_properties = gcnew List< PropertyWrapper^ >();
	}

public:

	HierarchicalPropertyWrapper( HierarchicalPropertyWrapper^ parent, const char* name )
		:	PropertyWrapper( parent, PropertyType::PropertyActor, RTTRPropertyRapist::MakeProperty( nullptr ), name )
		,	m_wrappedObject( nullptr )
	{
		m_properties = gcnew List< PropertyWrapper^ >();
	}

	/**@brief Initialize property without parent (on top level hierarchy)*/
	HierarchicalPropertyWrapper( const rttr::instance& objectPtr, const char* name )
		:	PropertyWrapper( nullptr, PropertyType::PropertyActor, RTTRPropertyRapist::MakeProperty( nullptr ), name )
		,	m_wrappedObject( nullptr )
	{
		m_properties = gcnew List< PropertyWrapper^ >();
		SetWrappedObject( objectPtr );
	}

	~HierarchicalPropertyWrapper();


	virtual void			BuildHierarchy		( rttr::type classType, BuildContext& context );
	void					BuildHierarchy		( BuildContext& context );

	const rttr::instance&	GetWrappedObject	() { return *m_wrappedObject; }

protected:

	PropertyWrapper^		BuildProperty				( HierarchicalPropertyWrapper^ parent, rttr::property property, BuildContext& context );
	PropertyWrapper^		TryBuildArithmeticProperty	( HierarchicalPropertyWrapper^ parent, rttr::property property );

	void					AddPropertyChild			( PropertyWrapper^ child );
	void					SetWrappedObject			( const rttr::instance& owner );

public:

	property List< PropertyWrapper^ >^	Properties
	{
		List< PropertyWrapper^ >^		get() { return m_properties; }
	}
};

}
}

