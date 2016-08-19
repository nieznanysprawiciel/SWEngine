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
