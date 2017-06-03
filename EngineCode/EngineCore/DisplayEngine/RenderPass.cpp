#include "EngineCore/stdafx.h"
#include "EngineCore/DisplayEngine/RenderPass.h"

#include "swGraphicAPI//Resources/MeshResources.h"

#include "swCommonLib/Common/MemoryLeaks.h"


// ================================ //
//
RenderPassDepracated::RenderPassDepracated()
	:	m_camera( nullptr ),
		m_renderTarget( nullptr ),
		m_bufferLayout( nullptr )
{}


// ================================ //
//
RenderPassDepracated::~RenderPassDepracated()
{
	if( m_renderTarget )
		m_renderTarget->DeleteObjectReference();
	if( m_bufferLayout )
		m_bufferLayout->DeleteObjectReference();
}

// ================================ //
//
void RenderPassDepracated::AddMesh( StaticActor* mesh )
{
	m_meshes.push_back( mesh );
}

// ================================ //
//
void RenderPassDepracated::SetRenderTarget( RenderTargetObject* renderTarget )
{
	if( m_renderTarget )
		m_renderTarget->DeleteObjectReference();
	
	m_renderTarget = renderTarget;
	if( renderTarget )
		m_renderTarget->AddObjectReference();
}

// ================================ //
//
void RenderPassDepracated::SetLayout( ShaderInputLayout* layout )
{
	if( m_bufferLayout )
		m_bufferLayout->DeleteObjectReference();
	
	m_bufferLayout = layout;
	if( layout )
		m_bufferLayout->AddObjectReference();
}
