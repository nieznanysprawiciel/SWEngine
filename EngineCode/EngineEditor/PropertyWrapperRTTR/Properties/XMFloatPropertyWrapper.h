#pragma once
/**
@file XMFloatPropertyWrapper.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/

#include "EngineEditor/PropertyWrapperRTTR/Properties/Complex/HierarchicalPropertyWrapper.h"



namespace sw {
namespace EditorPlugin
{


/**@brief Property for types DirectX::XMFLOAT2, XMFLOAT3, XMFLOAT4.*/
public ref class XMFloatPropertyWrapper : HierarchicalPropertyWrapper
{
private:
public:
	XMFloatPropertyWrapper( HierarchicalPropertyWrapper^ parent, rttr::property prop )
		: HierarchicalPropertyWrapper( parent, GetPropertyType( prop ), prop, prop.get_name().to_string().c_str() )
	{}
};





}
}
