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
ArrayElementPropertyWrapper::ArrayElementPropertyWrapper( ArrayPropertyWrapper^ parent, const std::string& name )
	:	HierarchicalPropertyWrapper( parent, PropertyType::PropertyUnknown, RTTRPropertyRapist::MakeProperty( nullptr ), name.c_str() )
{
	m_expandProperty = false;
}


// ================================ //
//
void		ArrayElementPropertyWrapper::BuildHierarchy		( const rttr::instance& element, BuildContext& context )
{
	rttr::instance elementInstance = element.get_type().get_raw_type().is_wrapper() ? element.get_wrapped_instance() : element;
	BuildHierarchy( element.get_type(), context );
}

}	// EditorPlugin
}	// sw

