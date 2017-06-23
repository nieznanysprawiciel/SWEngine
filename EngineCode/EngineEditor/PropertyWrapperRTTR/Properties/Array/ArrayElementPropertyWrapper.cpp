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
ArrayElementPropertyWrapper::ArrayElementPropertyWrapper( void* parent, const std::string& name )
	: CategoryLessPropertyWrapper( parent, PropertyType::PropertyUnknown, RTTRPropertyRapist::MakeProperty( nullptr ), name.c_str() )
{
	m_expandProperty = false;
}


// ================================ //
//
void		ArrayElementPropertyWrapper::BuildHierarchy		( rttr::type elementType )
{
	BuildHierarchy( m_actorPtr, elementType );
}

}	// EditorPlugin
}	// sw

