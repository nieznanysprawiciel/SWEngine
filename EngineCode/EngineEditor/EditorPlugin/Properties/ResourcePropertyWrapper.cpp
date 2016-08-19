#pragma warning(disable : 4561)	// DirectXMath converting fastcall to stdcall
#include "ResourcePropertyWrapper.h"

#include "GraphicAPI/MeshResources.h"
#include "EngineCore/ModelsManager/Model3DFromFile.h"



namespace EditorPlugin
{

/**@brief */
	ResourcePropertyWrapper::ResourcePropertyWrapper( void* parent, rttr::property prop )
	: CategoryLessPropertyWrapper( parent, PropertyType::PropertyResource, prop, prop.get_name().c_str() )
{
	auto type = rttr::type::get( prop );

	if( type.is_derived_from< Model3DFromFile >() )
		m_resourceType = ResourcePropertyType::PropertyMesh;
	else if( type.is_derived_from< BufferObject >() )
		m_resourceType = ResourcePropertyType::PropertyBuffer;
	else if( type.is_derived_from< TextureObject >() )
		m_resourceType = ResourcePropertyType::PropertyTexture;
	else if( type.is_derived_from< MaterialObject >() )
		m_resourceType = ResourcePropertyType::PropertyMaterial;
	else if( type.is_derived_from< IShader >() )
		m_resourceType = ResourcePropertyType::PropertyShader;
	else if( type.is_derived_from< RenderTargetObject >() )
		m_resourceType = ResourcePropertyType::PropertyRenderTarget;
	else
		m_resourceType = ResourcePropertyType::PropertyUnKnown;
}


}	// EditorPlugin
