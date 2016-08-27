#pragma once

#include "PropertyWrapper.h"

namespace EditorPlugin
{




public ref class ArrayPropertyWrapper : CategoryLessPropertyWrapper
{
private:
protected:
	bool		m_isDynamic;
	uint32		m_arraySize;
public:
	ArrayPropertyWrapper		( void* parent, rttr::property prop );


	property bool			IsDynamic
	{
		bool				get();
	}

	property uint32			ArraySize
	{
		uint32				get();
	}

	void					BuildHierarchy	( void* parent, rttr::type classType ) override;
	void					BuildHierarchy	();
};


}	// EditorPlugin
