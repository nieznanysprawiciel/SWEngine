#pragma once
/**
@file StringPropertyWrapper.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/

#include "EngineEditor/PropertyWrapperRTTR/Properties/Base/PropertyWrapper.h"


namespace sw {
namespace EditorPlugin
{


//====================================================================================//
//				StringPropertyWrapper
//====================================================================================//



/**@brief Property typu std::string.*/
public ref class StringPropertyWrapper : PropertyWrapper
{
public:
	StringPropertyWrapper( HierarchicalPropertyWrapper^ parent, rttr::property prop )
		: PropertyWrapper( parent, PropertyType::PropertyString, prop, prop.get_name().to_string().c_str() )
	{}

	StringPropertyWrapper( HierarchicalPropertyWrapper^ parent, rttr::property prop, const char* name )
		: PropertyWrapper( parent, PropertyType::PropertyString, prop, name )
	{}

public:
	property System::String^		Value
	{
		System::String^		get	();
		void				set	( System::String^ value );
	}

};


//====================================================================================//
//				WStringPropertyWrapper
//====================================================================================//



/**@brief Property typu std::wstring.*/
public ref class WStringPropertyWrapper : PropertyWrapper
{
public:
	WStringPropertyWrapper( HierarchicalPropertyWrapper^ parent, rttr::property prop )
		: PropertyWrapper( parent, PropertyType::PropertyWString, prop, prop.get_name().to_string().c_str() )
	{}

	WStringPropertyWrapper( HierarchicalPropertyWrapper^ parent, rttr::property prop, const char* name )
		: PropertyWrapper( parent, PropertyType::PropertyWString, prop, name )
	{}

public:
	property System::String^		Value
	{
		System::String^		get	();
		void				set	( System::String^ value );
	}

};



}
}	// sw


