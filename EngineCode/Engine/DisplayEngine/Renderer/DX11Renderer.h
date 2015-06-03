#pragma once

#include "DX11_interfaces_container.h"
#include "ModelsManager\meshes_textures_materials.h"
#include "DisplayEngine\ConstantBuffersFormat.h"


typedef DX11Renderer Renderer;
#include "Renderer.h"		/// It's stupid because this file includes us, but there're some important definitions.



/**@brief Renderer obs³uguj¹cy DirectX 11.*/
class DX11Renderer	:	public DX11_interfaces_container
{
private:
	/**Kontekst urz¹dzenia s³u¿¹cy do renderowania. W zale¿noœci czy stosujemy opóŸnione renderowanie (deferred rendering) czy nie,
	ta zmienna przechowuje kopiê wskaŸnika z DX11_interfaces_container lub opóŸniony kontekst (deferred context).
	W trakcie renderowania nie musimy wiedzieæ z czym mamy do czynienia.*/
	ID3D11DeviceContext*		local_device_context;			
	bool						valid;					///< Je¿eli klasa nie zosta³a zainicjowana poprawnie, to bêdzie tu wartoœæ false.
public:
	DX11Renderer( RENDERER_USAGE usage = USE_AS_IMMEDIATE );
	~DX11Renderer();

	inline bool supportsDefferedContextRendering() { return false; }		///< Informuje czy Renderer nadaje siê do renderowania w wielu w¹tkach do opóŸnionego contextu.
	inline bool isValid() { return valid; }


	void setTextures( const ModelPart& model );
	void setIndexBuffer( BufferObject* buffer );
	bool setVertexBuffer( BufferObject* buffer );
	void depthBufferEnable( bool state );
};



#include "DX11Renderer.inl"
