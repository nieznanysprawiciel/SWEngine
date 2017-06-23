#pragma once
/**
@file CategoryWrapper.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/

#include "EngineEditor/PropertyWrapperRTTR/Properties/Base/PropertyWrapper.h"

#include "EngineEditor/PropertyWrapperRTTR/Properties/Base/PropertyHelper.h"


namespace sw {
namespace EditorPlugin
{


using namespace System::Collections::Generic;



/**@brief This class wrappes child properties into categories.

*/
public ref class CategoryWrapper : PropertyWrapper
{
private:

	List< PropertyWrapper^ >^			m_properties;

protected:

	CategoryWrapper( void* parent, PropertyType type, rttr::property prop, const char* name )
		: PropertyWrapper( parent, type, prop, name )
	{
		m_properties = gcnew List< PropertyWrapper^ >();
	}

public:
	CategoryWrapper( void* parent, const char* name )
		: PropertyWrapper( parent, PropertyType::PropertyCategory, RTTRPropertyRapist::MakeProperty( nullptr ), name )
	{
		m_properties = gcnew List< PropertyWrapper^ >();
	}

	virtual void			BuildHierarchy	( void* objectPtr, rttr::type classType );

protected:
	PropertyWrapper^		BuildProperty	( void* parent, rttr::property property );

public:
	property List< PropertyWrapper^ >^	Properties
	{
		List< PropertyWrapper^ >^		get() { return m_properties; }
	}
};


}
}
