#include "ArrayElementPropertyWrapper.h"



namespace EditorPlugin
{



// ================================ //
//
ArrayElementPropertyWrapper::ArrayElementPropertyWrapper( void* parent, const std::string& name )
	:	CategoryLessPropertyWrapper( parent, PropertyType::PropertyUnknown, RTTRPropertyRapist::MakeProperty( nullptr ), name.c_str() )
{}


// ================================ //
//
void		ArrayElementPropertyWrapper::BuildHierarchy		( rttr::type elementType )
{
	BuildHierarchy( m_actorPtr, elementType );
}

}	// EditorPlugin

