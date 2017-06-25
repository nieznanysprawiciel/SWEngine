#pragma once
/**
@file ArrayPropertyWrapper.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/



#include "EngineEditor/PropertyWrapperRTTR/Properties/Complex/HierarchicalPropertyWrapper.h"



namespace sw {
namespace EditorPlugin
{



/**@brief Wrappes array types like std::vector or raw c++ arrays.*/
public ref class ArrayPropertyWrapper : HierarchicalPropertyWrapper
{
private:
protected:

	bool		m_isDynamic;
	uint32		m_arraySize;

public:
	ArrayPropertyWrapper		( HierarchicalPropertyWrapper^ parent, rttr::property prop );


	property bool			IsDynamic
	{
		bool				get();
	}

	property uint32			ArraySize
	{
		uint32				get();
	}

	void					BuildHierarchy	( const rttr::instance& parent, rttr::type classType ) override;
	void					BuildHierarchy	();
};


}	// EditorPlugin
}	// sw
