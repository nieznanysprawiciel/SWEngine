#pragma once
/**
@file ResourcePropertyWrapper.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/


#include "EngineEditor/PropertyWrapperRTTR/Properties/Complex/HierarchicalPropertyWrapper.h"

namespace sw {
namespace EditorPlugin
{


public enum class ResourcePropertyType
{
	Mesh,
	Animation,
	Skeleton,
	Buffer,
	Material,
	Texture,
	Shader,
	RenderTarget,
	UnknownType

};

/**@brief Property dla obiektów zaPropertyAnimationsobów: Model3DFromFile, BufferObject, MaterialObject, TextureObject*/
public ref class ResourcePropertyWrapper : HierarchicalPropertyWrapper
{
protected:

	ResourcePropertyType			m_resourceType;

public:
	ResourcePropertyWrapper			( PropertyWrapper^ parent, rttr::property prop );

	property ResourcePropertyType	ResourceType
	{
		ResourcePropertyType		get ()
		{
			return m_resourceType;
		}

		void						set ( ResourcePropertyType type )
		{
			m_resourceType = type;
		}
	}

};


}	// EditorPlugin
}	// sw
