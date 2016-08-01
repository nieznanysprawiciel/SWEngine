#pragma once
/**
@file DX11Renderer.h
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.

Deklaracja renderera u¿ywaj¹cego DirectX 11.
*/


#include "../../DX11API/DX11Initializer/DX11APIObjects.h"
#include "../../MeshResources.h"
#include "../../IRenderer.h"

class DX11Renderer;
typedef DX11Renderer Renderer;




/**@brief Renderer obs³uguj¹cy DirectX 11.
@ingroup DX11API*/
class DX11Renderer	: public IRenderer,	public DX11AuxiliaryObjects
{
private:
	/**Kontekst urz¹dzenia s³u¿¹cy do renderowania. W zale¿noœci czy stosujemy opóŸnione renderowanie (deferred rendering) czy nie,
	ta zmienna przechowuje kopiê wskaŸnika z DX11_interfaces_container lub opóŸniony kontekst (deferred context).
	W trakcie renderowania nie musimy wiedzieæ z czym mamy do czynienia.*/
	ID3D11DeviceContext*		m_localDeviceContext;			
	bool						m_valid;					///< Je¿eli klasa nie zosta³a zainicjowana poprawnie, to bêdzie tu wartoœæ false.
	RendererUsage				m_usageType;				///< Informacja jak bêdzie u¿ywany rederer.
public:
	DX11Renderer( RendererUsage usage = USE_AS_IMMEDIATE );
	~DX11Renderer();

	bool SupportsDefferedContextRendering() override	{ return true; }		///< Informuje czy Renderer nadaje siê do renderowania w wielu w¹tkach do opóŸnionego contextu.
	bool IsValid() override								{ return m_valid; }


	void SetTextures								( const ModelPart& model )																override;
	void SetIndexBuffer								( BufferObject* buffer )																override;
	bool SetVertexBuffer							( BufferObject* buffer, unsigned int offset = 0 )										override;
	void DepthBufferEnable							( bool state )																			override;
	void SetDefaultVertexLayout						()																						override;
	void SetDefaultSampler							()																						override;

	void InitBuffers								( unsigned int sizePerFram, unsigned int sizePerMesh )									override;
	void InitDepthStates							()																						override;

	void IASetPrimitiveTopology						( PrimitiveTopology topology )															override;
	void IASetInputLayout							( ShaderInputLayout* pInputLayout )														override;


	void VSSetConstantBuffers						( uint32 slot, BufferObject* buffer )													override;
	void PSSetConstantBuffers						( uint32 slot, BufferObject* buffer )													override;

	void UpdateSubresource							( BufferObject* pDstResource, const void *pSrcData )									override;

	void SetShaders									( ModelPart& model )																	override;

	// Rendering
	void Draw										( unsigned int vertexCount, unsigned int startVertexLocation )							override;
	void DrawIndexed								( unsigned int indexCount, unsigned int startIndexLocation, int baseVertexLocation )	override;
	
	void Present									()																						override;
	void BeginScene									( RenderTargetObject* mainRenderTarget )												override;
};



