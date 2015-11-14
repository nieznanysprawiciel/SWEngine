#include "EngineCore/stdafx.h"
#include "EngineCore/DisplayEngine/RenderPass.h"

void RenderPass::AddMesh( DynamicMeshObject* mesh )
{
	m_meshes.push_back( mesh );
}

void RenderPass::SetRenderTarget( RenderTargetObject* renderTarget )
{
	m_renderTarget = renderTarget;
}

void RenderPass::SetLayout( ShaderInputLayoutObject* layout )
{
	m_bufferLayout = layout;
}
