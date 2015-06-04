#pragma once

#include "DX11_interfaces_container.h"
#include "ModelsManager\meshes_textures_materials.h"
#include "DisplayEngine\ConstantBuffersFormat.h"

class DX11Renderer;
typedef DX11Renderer Renderer;
#include "RendererConstants.h"


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
	ID3D11DeviceContext*		local_device_context;			
	bool						valid;					///< Je¿eli klasa nie zosta³a zainicjowana poprawnie, to bêdzie tu wartoœæ false.
	RENDERER_USAGE				usage_type;				///< Informacja jak bêdzie u¿ywany rederer.
public:
	DX11Renderer( RENDERER_USAGE usage = USE_AS_IMMEDIATE );
	~DX11Renderer();

	inline bool supportsDefferedContextRendering() { return true; }		///< Informuje czy Renderer nadaje siê do renderowania w wielu w¹tkach do opóŸnionego contextu.
	inline bool isValid() { return valid; }


	void setTextures( const ModelPart& model );
	void setIndexBuffer( BufferObject* buffer );
	bool setVertexBuffer( BufferObject* buffer, unsigned int offset = 0 );
	void depthBufferEnable( bool state );
	void setDefaultVertexLayout();
	void setDefaultSampler();

	void initBuffers( unsigned int size_per_frame, unsigned int size_per_mesh );
	void initDepthStates();

	void IASetPrimitiveTopology( D3D_PRIMITIVE_TOPOLOGY topology );
	void IASetInputLayout( ID3D11InputLayout *pInputLayout );


	void VSSetConstantBuffers( UINT StartSlot, UINT NumBuffers, ID3D11Buffer *const *ppConstantBuffers );
	void VSSetConstantBuffers( UINT StartSlot, UINT NumBuffers, BufferObject* buffer );

	void PSSetConstantBuffers( UINT StartSlot, UINT NumBuffers, ID3D11Buffer *const *ppConstantBuffers );
	void PSSetConstantBuffers( UINT StartSlot, UINT NumBuffers, BufferObject* buffer );

	void UpdateSubresource( ID3D11Resource *pDstResource, const void *pSrcData );

	void setShaders( ModelPart& model );

	// Rendering
	void Draw( unsigned int VertexCount, unsigned int StartVertexLocation );
	void DrawIndexed( unsigned int IndexCount, unsigned int StartIndexLocation, int BaseVertexLocation );
};



#include "DX11Renderer.inl"
