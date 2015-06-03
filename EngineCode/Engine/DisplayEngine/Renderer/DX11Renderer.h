#pragma once

#include "DX11_interfaces_container.h"
#include "ModelsManager\meshes_textures_materials.h"
#include "DisplayEngine\ConstantBuffersFormat.h"


typedef DX11Renderer Renderer;
#include "Renderer.h"		/// It's stupid because this file includes us, but there're some important definitions.



/**@brief Renderer obs�uguj�cy DirectX 11.*/
class DX11Renderer	:	public DX11_interfaces_container
{
private:
	/**Kontekst urz�dzenia s�u��cy do renderowania. W zale�no�ci czy stosujemy op�nione renderowanie (deferred rendering) czy nie,
	ta zmienna przechowuje kopi� wska�nika z DX11_interfaces_container lub op�niony kontekst (deferred context).
	W trakcie renderowania nie musimy wiedzie� z czym mamy do czynienia.*/
	ID3D11DeviceContext*		local_device_context;			
	bool						valid;					///< Je�eli klasa nie zosta�a zainicjowana poprawnie, to b�dzie tu warto�� false.
public:
	DX11Renderer( RENDERER_USAGE usage = USE_AS_IMMEDIATE );
	~DX11Renderer();

	inline bool supportsDefferedContextRendering() { return false; }		///< Informuje czy Renderer nadaje si� do renderowania w wielu w�tkach do op�nionego contextu.
	inline bool isValid() { return valid; }


	void setTextures( const ModelPart& model );
	void setIndexBuffer( BufferObject* buffer );
	bool setVertexBuffer( BufferObject* buffer );
	void depthBufferEnable( bool state );
};



#include "DX11Renderer.inl"
