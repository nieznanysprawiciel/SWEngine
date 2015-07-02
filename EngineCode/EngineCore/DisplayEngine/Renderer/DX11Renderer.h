#pragma once

#include "DX11_interfaces_container.h"
#include "ModelsManager\meshes_textures_materials.h"
#include "DisplayEngine\ConstantBuffersFormat.h"

class DX11Renderer;
typedef DX11Renderer Renderer;
#include "IRenderer.h"


#ifndef RENDERER_H_INCLUDE
	#error Plik DX11Renderer.h zosta� zaincludowany bezpo�rednio. Zaincluduj plik Renderer.h zamiast niego i ustaw makro ENABLE_RENDERER.
#endif

/**@brief Renderer obs�uguj�cy DirectX 11.*/
class DX11Renderer	:	public DX11_constant_buffers_container
{
private:
	/**Kontekst urz�dzenia s�u��cy do renderowania. W zale�no�ci czy stosujemy op�nione renderowanie (deferred rendering) czy nie,
	ta zmienna przechowuje kopi� wska�nika z DX11_interfaces_container lub op�niony kontekst (deferred context).
	W trakcie renderowania nie musimy wiedzie� z czym mamy do czynienia.*/
	ID3D11DeviceContext*		m_localDeviceContext;			
	bool						m_valid;					///< Je�eli klasa nie zosta�a zainicjowana poprawnie, to b�dzie tu warto�� false.
	RENDERER_USAGE				m_usageType;				///< Informacja jak b�dzie u�ywany rederer.
public:
	DX11Renderer( RENDERER_USAGE usage = USE_AS_IMMEDIATE );
	~DX11Renderer();

	inline bool SupportsDefferedContextRendering() { return true; }		///< Informuje czy Renderer nadaje si� do renderowania w wielu w�tkach do op�nionego contextu.
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
