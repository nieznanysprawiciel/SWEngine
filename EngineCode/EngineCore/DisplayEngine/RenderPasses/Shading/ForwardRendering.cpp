#include "EngineCore/stdafx.h"
/**
@file ForwardRendering.cpp
@author nieznanysprawiciel
@copyright File is part of graphic engine SWEngine.
*/

#include "ForwardRendering.h"
#include "EngineCore/DisplayEngine/RenderPasses/RenderingHelpers.h"


using namespace DirectX;


// ================================ //
//
bool		ForwardRendering::PreRender		( IRenderer* renderer, RenderContext& context )
{
	RenderingHelper::UpdateCameraBuffer( m_mainCamera, renderer, context );
	RenderingHelper::UpdateLightBuffer( renderer, context );



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
		StaticActor* object = meshes[i];


		// Ustawiamy bufor wierzcho³ków
		if ( renderer->SetVertexBuffer( object->GetVertexBuffer() ) )
			continue;	// Je¿eli nie ma bufora wierzcho³ków, to idziemy do nastêpnego mesha

		// Ustawiamy bufor indeksów, je¿eli istnieje
		renderer->SetIndexBuffer( object->GetIndexBuffer() );


#ifdef _INTERPOLATE_POSITIONS
		XMMATRIX transformation = XMLoadFloat4x4( &( context.Transforms[ i ] ) );
#else
		XMVECTOR translation = XMLoadFloat3( &(object->position) );
		XMVECTOR orientation = XMLoadFloat4( &(object->orientation) );
		XMMATRIX transformation = XMMatrixRotationQuaternion( orientation );
		transformation = transformation * XMMatrixTranslationFromVector( translation );
#endif


		for ( unsigned int j = 0; j < object->GetModelParts().size( ); ++j )
		{
			ModelPart& model = object->GetModelParts()[j];

			// Compute transformation matrix for segment.
			XMMATRIX worldTransform;
			worldTransform = XMLoadFloat4x4( &(model.mesh->transform_matrix) );
			worldTransform = worldTransform * transformation;

			// Fill transform constants buffer.
			TransformConstants meshTransformData;
			XMStoreFloat4x4( &meshTransformData.WorldMatrix, XMMatrixTranspose( worldTransform ) );
			XMStoreFloat4( &meshTransformData.MeshScale, XMVectorSetW( XMVectorReplicate( object->GetScale() ), 1.0f ) );

			RenderingHelper::UpdateBuffer( renderer, context.TransformBuffer, meshTransformData );

			// Update material data.
			ConstantPerMesh materialData;
			context.DisplayEngine->CopyMaterial( &materialData, &model );

			RenderingHelper::UpdateBuffer( renderer, context.MaterialConstants, materialData );



			SetRenderStateCommand renderCommand;
			renderCommand.TransformBuffer = context.TransformBuffer;
			renderCommand.MaterialBuffer = context.MaterialConstants;
			renderCommand.BonesTransforms = nullptr;
			//renderCommand.VertexShader


			// Ustawiamy shadery
			renderer->SetShaders( model );


			// Ustawiamy tekstury
			renderer->SetTextures( model );

			// Teraz renderujemy. Wybieramy albo tryb indeksowany, albo bezpoœredni.
			MeshPartObject* part = model.mesh;
			if ( part->use_index_buf )
				renderer->DrawIndexed( part->vertices_count, part->buffer_offset, part->base_vertex );
			else // Tryb bezpoœredni
				renderer->Draw( part->vertices_count, part->buffer_offset );
		}

	}

}

// ================================ //
//
void		ForwardRendering::PostRender	( IRenderer* renderer, RenderContext& context )
{

}

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
