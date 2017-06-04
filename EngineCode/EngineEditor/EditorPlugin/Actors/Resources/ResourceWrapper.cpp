/**
@file ResourceWrapper.cpp
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/


#include "ResourceWrapper.h"


#include "swGraphicAPI/Resources/MeshResources.h"
#include "EngineCore/ModelsManager/Assets/Meshes/MeshAsset.h"
#include "EngineCore/ModelsManager/Assets/Materials/MaterialAsset.h"




namespace sw {
namespace EditorPlugin
{



// ================================ //
//
ResourceWrapper::ResourceWrapper( ResourceObject* resource )
	: EngineObjectWrapper( resource )
{
	auto type = resource->GetType();

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


/**@brief */
int					ResourceWrapper::ID::get()
{
	if( m_actorPtr )
		return static_cast<ResourceObject*>( m_actorPtr )->GetID();
	return std::numeric_limits< uint32 >::max();
}

/**@brief */
System::String^		ResourceWrapper::ResourceName::get()
{
	if( m_actorPtr )
		return gcnew System::String( static_cast<ResourceObject*>( m_actorPtr )->GetResourceName().c_str() );
	return gcnew System::String( "" );
}

}	// EditorPlugin
}	// sw
