#include "EngineCore/stdafx.h"
/**
@file ForwardRendering.cpp
@author nieznanysprawiciel
@copyright File is part of graphic engine SWEngine.
*/

#include "ForwardRendering.h"
#include "EngineCore/DisplayEngine/RenderPasses/RenderingHelpers.h"

#include "EngineCore/ModelsManager/AssetsManager.h"

#include <algorithm>


using namespace DirectX;


// ================================ //
//
ForwardRendering::ForwardRendering()
{
	m_blendingState = s_assetsManager->GetBlendingState( DefaultAssets::DEFAULT_BLENDING_STATE_STRING );
	m_rasterizer = s_assetsManager->GetRasterizerState( DefaultAssets::DEFAULT_RASTERIZER_STATE_STRING );
	m_depthState = s_assetsManager->GetDepthStencilState( DefaultAssets::DEFAULT_DEPTH_STATE_STRING );

	// If someone removed states from AssetsManager, these asserts will fail.
	assert( m_blendingState );
	assert( m_rasterizer );
	assert( m_rasterizer );
}

// ================================ //
//
ForwardRendering::~ForwardRendering()
{
	// Release resources in future.
}


// ================================ //
//
bool		ForwardRendering::PreRender		( IRenderer* renderer, RenderContext& context )
{
	RenderingHelper::UpdateCameraBuffer( m_mainCamera, renderer, context );
	RenderingHelper::UpdateLightBuffer( renderer, context );

	RenderingHelper::ClearRenderTargetAndDepth( renderer, m_mainTarget.Ptr(), DirectX::XMFLOAT4( 0.0f, 0.0f, 0.0f, 0.0f ), 1.0f );

	SetRenderTargetCommand renderTargetCommand;
	renderTargetCommand.CameraBuffer = context.CameraBuffer;
	renderTargetCommand.LightBuffer = context.LightBuffer;
	renderTargetCommand.DepthStencil = m_mainTarget.Ptr();
	renderTargetCommand.RenderTargets[ 0 ] = m_mainTarget.Ptr();
	std::fill( renderTargetCommand.RenderTargets + 1, renderTargetCommand.RenderTargets + MAX_BOUND_RENDER_TARGETS, nullptr );

	renderTargetCommand.BlendingState = m_blendingState.Ptr();
	renderTargetCommand.DepthStencilState = m_depthState.Ptr();
	renderTargetCommand.RasterizerState = m_rasterizer.Ptr();

	renderer->SetRenderTarget( renderTargetCommand );

	return true;
}

// ================================ //
//
void		ForwardRendering::Render		( IRenderer* renderer, RenderContext& context, Size rangeStart, Size rangeEnd )
{
	auto& meshes = context.DisplayEngine->GetMeshes();

	//na razie pêtla bez optymalizacji
	for ( auto i = rangeStart; i < std::min( meshes.size(), rangeEnd ); ++i )
	{
		StaticActor* object = meshes[ i ];


#ifdef _INTERPOLATE_POSITIONS
		XMMATRIX transformation = XMLoadFloat4x4( &( context.Transforms[ i ] ) );
#else
		XMVECTOR translation = XMLoadFloat3( &(object->position) );
		XMVECTOR orientation = XMLoadFloat4( &(object->orientation) );
		XMMATRIX transformation = XMMatrixRotationQuaternion( orientation );
		transformation = transformation * XMMatrixTranslationFromVector( translation );
#endif

		auto meshAsset = object->GetModel().Ptr();

		for ( unsigned int j = 0; j < meshAsset->GetSegmentsCount(); ++j )
		{
			const MeshPart* model = meshAsset->GetSegment( j );


			// Fill transform constants buffer.
			TransformConstants meshTransformData;
			XMStoreFloat4x4( &meshTransformData.WorldMatrix, XMMatrixTranspose( transformation ) );
			XMStoreFloat4( &meshTransformData.MeshScale, XMVectorSetW( XMVectorReplicate( object->GetScale() ), 1.0f ) );

			RenderingHelper::UpdateBuffer( renderer, context.TransformBuffer, meshTransformData );


			auto material = model->Material.Ptr();

			SetRenderStateCommand renderCommand;
			renderCommand.TransformBuffer = context.TransformBuffer;
			renderCommand.MaterialBuffer = material->GetMaterialBuffer().Ptr();
			renderCommand.BonesTransforms = nullptr;
			renderCommand.VertexShader = material->GetVertexShader().Ptr();
			renderCommand.PixelShader = material->GetPixelShader().Ptr();

			for( int i = 0; i < ENGINE_MAX_TEXTURES; ++i )
			{
				renderCommand.Textures[ i ] = material->GetTexture( i ).Ptr();
				renderCommand.BindToShader[ i ] = (uint8)ShaderType::PixelShader;
			}

			renderer->SetShaderState( renderCommand );

			// Send draw command.
			DrawCommand drawCommand;
			drawCommand.BaseVertex = model->BaseVertex;
			drawCommand.BufferOffset = model->BufferOffset;
			drawCommand.NumVertices = model->NumVertices;
			drawCommand.Topology = model->Topology;
			drawCommand.VertexBuffer = meshAsset->GetVertexBufferRawPtr();
			drawCommand.IndexBufer = meshAsset->GetIndexBufferRawPtr();
			drawCommand.ExtendedIndex = model->Flags & MeshPartFlags::Use4BytesIndex ? true : false;
			drawCommand.Layout = meshAsset->GetLayoutRawPtr();

			renderer->Draw( drawCommand );
		}

	}

}

// ================================ //
//
void		ForwardRendering::PostRender	( IRenderer* renderer, RenderContext& context )
{}

/**@brief This function does nothing. No nested passes.*/
void		ForwardRendering::NestedPasses	( std::vector< Ptr< IRenderPass > >& passes )
{}

// ================================ //
//
void		ForwardRendering::SetMainCamera	( CameraActor* camera )
{
	m_mainCamera = camera;
}

// ================================ //
//
CameraActor*	ForwardRendering::GetMainCamera()
{
	return m_mainCamera;
}

// ================================ //
//
void		ForwardRendering::SetMainRenderTarget	( const ResourcePtr< RenderTargetObject >& target )
{
	m_mainTarget = target;
}

// ================================ //
//
ResourcePtr< RenderTargetObject >	ForwardRendering::GetMainRenderTarget()
{
	return m_mainTarget;
}
