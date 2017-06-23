#pragma once
/**
@file HierarchicalPropertyWrapper.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/

#include "EngineEditor/PropertyWrapperRTTR/Properties/Base/PropertyWrapper.h"


namespace sw {
namespace EditorPlugin
{

using namespace System::Collections::Generic;



/**@brief Base class for hierarchical properties like classes, structures (which have sub properties).
*/
public ref class HierarchicalPropertyWrapper : PropertyWrapper
{
private:

	List< PropertyWrapper^ >^			m_properties;

protected:

	HierarchicalPropertyWrapper( void* parent, PropertyType type, rttr::property prop, const char* name )
		: PropertyWrapper( parent, type, prop, name )
	{
		m_properties = gcnew List< PropertyWrapper^ >();
	}

public:

	HierarchicalPropertyWrapper( void* parent, const char* name )
		: PropertyWrapper( parent, PropertyType::PropertyCategory, RTTRPropertyRapist::MakeProperty( nullptr ), name )
	{
		m_properties = gcnew List< PropertyWrapper^ >();
	}



	virtual void			BuildHierarchy	( void* objectPtr, rttr::type classType );
	void					BuildHierarchy	();

protected:

	PropertyWrapper^		BuildProperty		( void* parent, rttr::property property );
	void					AddPropertyChild	( PropertyWrapper^ child );

public:

	property List< PropertyWrapper^ >^	Properties
	{
		List< PropertyWrapper^ >^		get() { return m_properties; }
	}
};

}
}

