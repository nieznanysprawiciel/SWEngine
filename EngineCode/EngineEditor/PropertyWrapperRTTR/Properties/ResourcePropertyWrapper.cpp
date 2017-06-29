#pragma warning(disable : 4561)	// DirectXMath converting fastcall to stdcall
#include "EngineEditor/PropertyWrapperRTTR/stdafx.h"


#include "ResourcePropertyWrapper.h"

#include "swGraphicAPI/Resources/MeshResources.h"
#include "EngineCore/ModelsManager/Assets/Meshes/MeshAsset.h"
#include "EngineCore/ModelsManager/Assets/Materials/MaterialAsset.h"



namespace sw {
namespace EditorPlugin
{

// ================================ //
//
ResourcePropertyWrapper::ResourcePropertyWrapper( HierarchicalPropertyWrapper^ parent, rttr::property prop )
	: HierarchicalPropertyWrapper( parent, PropertyType::PropertyResource, prop, prop.get_name().to_string().c_str() )
{
	auto type = rttr::type::get( prop );

	if( type.is_derived_from< MeshAsset >() )
		m_resourceType = ResourcePropertyType::Mesh;
	else if( type.is_derived_from< BufferObject >() )
		m_resourceType = ResourcePropertyType::Buffer;
	else if( type.is_derived_from< TextureObject >() )
		m_resourceType = ResourcePropertyType::Texture;
	else if( type.is_derived_from< MaterialAsset >() )
		m_resourceType = ResourcePropertyType::Material;
	else if( type.is_derived_from< IShader >() )
		m_resourceType = ResourcePropertyType::Shader;
	else if( type.is_derived_from< RenderTargetObject >() )
		m_resourceType = ResourcePropertyType::RenderTarget;
	else
		m_resourceType = ResourcePropertyType::UnknownType;
}


}	// EditorPlugin
}	// sw
