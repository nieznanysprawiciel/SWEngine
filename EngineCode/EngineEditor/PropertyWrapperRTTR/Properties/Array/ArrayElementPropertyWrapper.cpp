/**
@file ArrayElementPropertyWrapper.cpp
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/
#include "EngineEditor/PropertyWrapperRTTR/stdafx.h"

#include "ArrayElementPropertyWrapper.h"



namespace sw {
namespace EditorPlugin
{



// ================================ //
//
ArrayElementPropertyWrapper::ArrayElementPropertyWrapper( HierarchicalPropertyWrapper^ parent, const std::string& name )
	:	HierarchicalPropertyWrapper( parent, PropertyType::PropertyUnknown, RTTRPropertyRapist::MakeProperty( nullptr ), name.c_str() )
{
	m_expandProperty = false;
}


// ================================ //
//
void		ArrayElementPropertyWrapper::BuildHierarchy		( rttr::type elementType )
{
	BuildHierarchy( GetWrappedObject(), elementType );
}

}	// EditorPlugin
}	// sw

