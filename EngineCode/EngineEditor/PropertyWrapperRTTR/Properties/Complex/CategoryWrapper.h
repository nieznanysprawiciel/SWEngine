#pragma once
/**
@file CategoryWrapper.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/

#include "EngineEditor/PropertyWrapperRTTR/Properties/Complex/HierarchicalPropertyWrapper.h"
#include "EngineEditor/PropertyWrapperRTTR/Properties/Base/PropertyHelper.h"


namespace sw {
namespace EditorPlugin
{


using namespace System::Collections::Generic;



/**@brief This class wrappes child properties into categories.

*/
public ref class CategoryWrapper : HierarchicalPropertyWrapper
{
private:
protected:

	CategoryWrapper( void* parent, PropertyType type, rttr::property prop, const char* name )
		: HierarchicalPropertyWrapper( parent, type, prop, name )
	{}

public:

	CategoryWrapper( void* parent, const char* name )
		: HierarchicalPropertyWrapper( parent, PropertyType::PropertyCategory, RTTRPropertyRapist::MakeProperty( nullptr ), name )
	{}



	virtual void			BuildHierarchy	( void* objectPtr, rttr::type classType ) override;

};


}
}
