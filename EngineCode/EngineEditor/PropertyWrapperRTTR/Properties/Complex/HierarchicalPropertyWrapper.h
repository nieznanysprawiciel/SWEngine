#pragma once
/**
@file HierarchicalPropertyWrapper.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/

#include "EngineEditor/PropertyWrapperRTTR/Properties/Base/PropertyWrapper.h"

#include "swCommonLib/Common/RTTR.h"


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
	rttr::variant*					m_ownerPtr;

protected:

	HierarchicalPropertyWrapper( PropertyWrapper^ parent, PropertyType type, rttr::property prop, const char* name )
		:	PropertyWrapper( parent, type, prop, name )
		,	m_ownerPtr( new rttr::variant() )
	{
		m_properties = gcnew List< PropertyWrapper^ >();
	}

public:

	HierarchicalPropertyWrapper( PropertyWrapper^ parent, const char* name )
		:	PropertyWrapper( parent, PropertyType::PropertyCategory, RTTRPropertyRapist::MakeProperty( nullptr ), name )
		,	m_ownerPtr( new rttr::variant() )
	{
		m_properties = gcnew List< PropertyWrapper^ >();
	}

	~HierarchicalPropertyWrapper();


	virtual void			BuildHierarchy	( rttr::variant& objectPtr, rttr::type classType );
	void					BuildHierarchy	();

	rttr::variant			GetOwner		() { return *m_ownerPtr; }

protected:

	PropertyWrapper^		BuildProperty		( PropertyWrapper^ parent, rttr::property property );
	void					AddPropertyChild	( PropertyWrapper^ child );

	void					SetOwner			( rttr::variant& owner );

public:

	property List< PropertyWrapper^ >^	Properties
	{
		List< PropertyWrapper^ >^		get() { return m_properties; }
	}
};

}
}

