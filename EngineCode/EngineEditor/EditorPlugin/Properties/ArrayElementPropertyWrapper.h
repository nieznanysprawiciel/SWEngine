#pragma once


#include "PropertyWrapper.h"

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

