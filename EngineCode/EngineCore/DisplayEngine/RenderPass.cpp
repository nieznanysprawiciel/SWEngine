#include "EngineCore/stdafx.h"
#include "EngineCore/DisplayEngine/RenderPass.h"

#include "GraphicAPI/MeshResources.h"


RenderPass::RenderPass()
	:	m_camera( nullptr ),
		m_renderTarget( nullptr ),
		m_bufferLayout( nullptr )
{}

RenderPass::~RenderPass()
{
	if( m_renderTarget )
		m_renderTarget->DeleteObjectReference();
	if( m_bufferLayout )
		m_bufferLayout->DeleteObjectReference();
}

void RenderPass::AddMesh( StaticActor* mesh )
{
	m_meshes.push_back( mesh );
}

void RenderPass::SetRenderTarget( RenderTargetObject* renderTarget )
{
	if( m_renderTarget )
		m_renderTarget->DeleteObjectReference();
	
	m_renderTarget = renderTarget;
	if( renderTarget )
		m_renderTarget->AddObjectReference();
}

void RenderPass::SetLayout( ShaderInputLayout* layout )
{
	if( m_bufferLayout )
		m_bufferLayout->DeleteObjectReference();
	
	m_bufferLayout = layout;
	if( layout )
		m_bufferLayout->AddObjectReference();
}
