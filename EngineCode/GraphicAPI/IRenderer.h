#pragma once
/**
@file IRenderer.h
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.
*/


#include "GraphicAPI/MeshResources.h"

enum RendererUsage
{
	USE_AS_DEFERRED,
	USE_AS_IMMEDIATE
};



/**Klasa Renderera.*/
class IRenderer
{
private:
public:
	IRenderer( RendererUsage usage = USE_AS_IMMEDIATE );
	virtual ~IRenderer();


	virtual bool SupportsDefferedContextRendering() = 0;		///< Informuje czy Renderer nadaje siê do renderowania w wielu w¹tkach do opóŸnionego contextu.
	virtual bool IsValid() = 0;


	virtual void SetTextures						( const ModelPart& model ) = 0;
	virtual void SetIndexBuffer						( BufferObject* buffer ) = 0;
	virtual bool SetVertexBuffer					( BufferObject* buffer, unsigned int offset = 0 ) = 0;
	virtual void DepthBufferEnable					( bool state ) = 0;
	virtual void SetDefaultVertexLayout				() = 0;
	virtual void SetDefaultSampler					() = 0;

	virtual void InitBuffers						( unsigned int sizePerFrame, unsigned int sizePerMesh ) = 0;
	virtual void InitDepthStates					() = 0;

	virtual void IASetPrimitiveTopology				( PrimitiveTopology topology ) = 0;
	virtual void IASetInputLayout					( ShaderInputLayoutObject* inputLayout ) = 0;


	//virtual void VSSetConstantBuffers				( uint32 startSlot, uint32 numBuffers, ID3D11Buffer *const *ppConstantBuffers ) = 0;
	virtual void VSSetConstantBuffers				( uint32 slot, BufferObject* buffer ) = 0;

	//virtual void PSSetConstantBuffers				( uint32 startSlot, uint32 numBuffers, ID3D11Buffer *const *ppConstantBuffers ) = 0;
	virtual void PSSetConstantBuffers				( uint32 slot, BufferObject* buffer ) = 0;

	virtual void UpdateSubresource					( BufferObject* pDstResource, const void *pSrcData ) = 0;

	virtual void SetShaders							( ModelPart& model ) = 0;

	// Rendering
	virtual void Draw								( unsigned int vertexCount, unsigned int startVertexLocation ) = 0;
	virtual void DrawIndexed						( unsigned int indexCount, unsigned int startIndexLocation, int BaseVertexLocation ) = 0;

	virtual void Present							() = 0;
	virtual void BeginScene							() = 0;
};


