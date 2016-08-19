#pragma once

#include "PropertyWrapper.h"

namespace EditorPlugin
{


public enum class ResourcePropertyType
{
	PropertyMesh,
	PropertyAnimation,
	PropertySkeleton,
	PropertyBuffer,
	PropertyMaterial,
	PropertyTexture,
	PropertyShader,
	PropertyRenderTarget,
	PropertyUnKnown

};

/**@brief Property dla obiekt�w zasob�w: Model3DFromFile, BufferObject, MaterialObject, TextureObject*/
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
