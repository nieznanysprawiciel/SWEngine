#pragma once
/**
@file ArrayElementPropertyWrapper.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/


#include "EngineEditor/PropertyWrapperRTTR/Properties/Complex/HierarchicalPropertyWrapper.h"
#include "EngineEditor/PropertyWrapperRTTR/Properties/Array/ArrayPropertyWrapper.h"


namespace sw {
namespace EditorPlugin
{



public ref class ArrayElementPropertyWrapper : HierarchicalPropertyWrapper
{
private:
protected:
public:
	ArrayElementPropertyWrapper		( ArrayPropertyWrapper^ parent, const std::string& name );

	void		BuildHierarchy		( const rttr::instance& element, BuildContext& context );
};

}	// EditorPlugin
}	// sw

