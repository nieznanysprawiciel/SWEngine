#pragma once
/**
@file ObjectPropertyWrapper.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/

#include "EngineEditor/PropertyWrapperRTTR/Properties/Complex/HierarchicalPropertyWrapper.h"



namespace sw {
namespace EditorPlugin
{


/**@brief Property wrapper for classes.*/
public ref class ObjectPropertyWrapper : HierarchicalPropertyWrapper
{
private:
public:
	ObjectPropertyWrapper( void* parent, rttr::property prop )
		: HierarchicalPropertyWrapper( parent, PropertyType::PropertyActor, prop, prop.get_name().to_string().c_str() )
	{}
};



}
}
