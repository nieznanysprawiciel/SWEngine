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
	ArrayPropertyWrapper		( HierarchicalPropertyWrapper^ parent, rttr::property prop, const char* name );

public:

	property bool			IsDynamic
	{
		bool				get();
	}

	property uint32			ArraySize
	{
		uint32				get();
	}

public:

	virtual void			BuildProperties		( rttr::type classType, BuildContext& context ) override;
	virtual void			RebuildProperties	( rttr::variant& arrayVariant, BuildContext& context ) override;
	void					RebuildProperty		( rttr::variant& parent, BuildContext& context ) override;

private:

	void					Init				( HierarchicalPropertyWrapper^ parent, rttr::property prop );
};


}	// EditorPlugin
}	// sw
