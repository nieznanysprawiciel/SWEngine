#pragma once

#include "PropertyWrapper.h"

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
public ref class ResourcePropertyWrapper : CategoryLessPropertyWrapper
{
protected:

	ResourcePropertyType			m_resourceType;

public:
	ResourcePropertyWrapper			( void* parent, rttr::property prop );

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
