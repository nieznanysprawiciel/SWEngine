#pragma once

#include "DX11_interfaces_container.h"
#include "ModelsManager\meshes_textures_materials.h"
#include "DisplayEngine\ConstantBuffersFormat.h"

class DX11Renderer;
typedef DX11Renderer Renderer;
#include "IRenderer.h"


#ifndef RENDERER_H_INCLUDE
	#error Plik DX11Renderer.h zosta³ zaincludowany bezpoœrednio. Zaincluduj plik Renderer.h zamiast niego i ustaw makro ENABLE_RENDERER.
#endif

/**@brief Renderer obs³uguj¹cy DirectX 11.*/
class DX11Renderer	:	public DX11_constant_buffers_container
{
private:
	/**Kontekst urz¹dzenia s³u¿¹cy do renderowania. W zale¿noœci czy stosujemy opóŸnione renderowanie (deferred rendering) czy nie,
	ta zmienna przechowuje kopiê wskaŸnika z DX11_interfaces_container lub opóŸniony kontekst (deferred context).
	W trakcie renderowania nie musimy wiedzieæ z czym mamy do czynienia.*/
	ID3D11DeviceContext*		m_localDeviceContext;			
	bool						m_valid;					///< Je¿eli klasa nie zosta³a zainicjowana poprawnie, to bêdzie tu wartoœæ false.
	RENDERER_USAGE				m_usageType;				///< Informacja jak bêdzie u¿ywany rederer.
public:
	DX11Renderer( RENDERER_USAGE usage = USE_AS_IMMEDIATE );
	~DX11Renderer();

	inline bool SupportsDefferedContextRendering() { return true; }		///< Informuje czy Renderer nadaje siê do renderowania w wielu w¹tkach do opóŸnionego contextu.
	inline bool IsValid() { return m_valid; }


	void SetTextures( const ModelPart& model );
	void SetIndexBuffer( BufferObject* buffer );
	bool SetVertexBuffer( BufferObject* buffer, unsigned int offset = 0 );
	void DepthBufferEnable( bool state );
	void SetDefaultVertexLayout();
	void SetDefaultSampler();

	void InitBuffers( unsigned int sizePerFram, unsigned int sizePerMesh );
	void InitDepthStates();

	void IASetPrimitiveTopology( D3D_PRIMITIVE_TOPOLOGY topology );
	void IASetInputLayout( ID3D11InputLayout *pInputLayout );


	void VSSetConstantBuffers( UINT startSlot, UINT numBuffers, ID3D11Buffer *const *ppConstantBuffers );
	void VSSetConstantBuffers( UINT startSlot, UINT numBuffers, BufferObject* buffer );

	void PSSetConstantBuffers( UINT startSlot, UINT numBuffers, ID3D11Buffer *const *ppConstantBuffers );
	void PSSetConstantBuffers( UINT startSlot, UINT numBuffers, BufferObject* buffer );

	void UpdateSubresource( ID3D11Resource *pDstResource, const void *pSrcData );

	void SetShaders( ModelPart& model );

	// Rendering
	void Draw( unsigned int vertexCount, unsigned int startVertexLocation );
	void DrawIndexed( unsigned int indexCount, unsigned int startIndexLocation, int baseVertexLocation );
};



#include "DX11Renderer.inl"
