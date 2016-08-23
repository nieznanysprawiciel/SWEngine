#include "ResourceWrapper.h"


#include "GraphicAPI/MeshResources.h"
#include "EngineCore/ModelsManager/Model3DFromFile.h"

namespace EditorPlugin
{




ResourceWrapper::ResourceWrapper( ResourceObject* resource )
	:	EngineObjectWrapper( resource )
{
	auto type = resource->GetType();

	if( type.is_derived_from< Model3DFromFile >() )
		m_resourceType = ResourcePropertyType::Mesh;
	else if( type.is_derived_from< BufferObject >() )
		m_resourceType = ResourcePropertyType::Buffer;
	else if( type.is_derived_from< TextureObject >() )
		m_resourceType = ResourcePropertyType::Texture;
	else if( type.is_derived_from< MaterialObject >() )
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
		return static_cast< ResourceObject* >( m_actorPtr )->GetID();
	return std::numeric_limits< uint32 >::max();
}

/**@brief */
System::String^		ResourceWrapper::ResourceName::get()
{
	if( m_actorPtr )
		return gcnew System::String( static_cast< ResourceObject* >( m_actorPtr )->GetResourceName().c_str() );
	return gcnew System::String( "" );
}

}	// EditorPlugin
