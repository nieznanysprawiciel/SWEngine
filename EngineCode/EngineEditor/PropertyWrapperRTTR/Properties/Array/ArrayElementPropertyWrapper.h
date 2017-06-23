#pragma once
/**
@file ArrayElementPropertyWrapper.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/



#include "EngineEditor/PropertyWrapperRTTR/Properties/Base/PropertyWrapper.h"



namespace sw {
namespace EditorPlugin
{



public ref class ArrayElementPropertyWrapper : CategoryLessPropertyWrapper
{
private:
protected:
public:
	ArrayElementPropertyWrapper		( void* parent, const std::string& name );

	void		BuildHierarchy		( rttr::type elementType );
};

}	// EditorPlugin
}	// sw

