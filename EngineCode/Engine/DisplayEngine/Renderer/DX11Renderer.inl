#pragma once
#include "DX11Renderer.h"


/**@brief Klasa bêdzie renderowaæ w trybie immediate albo deferred.

Je¿eli ustawiono flagê D3D11_CREATE_DEVICE_SINGLETHREADED, to nie wolno podaæ w parametrze
RENDERER_USAGE::USE_AS_DEFERRED.

Mo¿na sprawdziæ czy klasa zosta³a zainicjowana poprawnie wywo³uj¹c funkcjê isValid.*/
DX11Renderer::DX11Renderer( RENDERER_USAGE usage )
{
	if ( usage == RENDERER_USAGE::USE_AS_IMMEDIATE )
	{
		local_device_context = device_context;
		valid = true;
	}
	else if ( usage == RENDERER_USAGE::USE_AS_DEFERRED )		// Will not work, if the device was created with the D3D11_CREATE_DEVICE_SINGLETHREADED value.
	{
		device->CreateDeferredContext( 0, &local_device_context );
		valid = true;
	}
	else
	{
		valid = false;
		assert( false );
	}
}


DX11Renderer::~DX11Renderer()
{
}


/**@brief Funkcja ustawia tesktury z danego ModelParta w DirectXie.

@param[in] model ModelPart z którego pochodz¹ tekstury do ustawienia.
@todo SetShaderResource mo¿na u¿yæ do ustawienia od razu ca³ej tablicy. Trzeba umo¿liwiæ ustawianie
do VS i innych.*/
inline void DX11Renderer::setTextures( const ModelPart& model )
{
	for ( int i = 0; i < ENGINE_MAX_TEXTURES; ++i )
		if ( model.texture[i] )		// Nie ka¿da tekstura w tablicy istnieje
		{
			ID3D11ShaderResourceView* tex = model.texture[i]->get();
			local_device_context->PSSetShaderResources( i, 1, &tex );
		}
}

/**@brief Ustawia w kontekœcie urz¹dzenia bufor indeksów.

@param[in] buffer Bufor do ustawienia.*/
inline void DX11Renderer::setIndexBuffer( BufferObject* buffer )
{
	// Ustawiamy bufor indeksów, je¿eli istnieje
	ID3D11Buffer* indexBuffer = nullptr;
	if ( buffer )
	{
		indexBuffer = buffer->get();
		unsigned int offset = 0;
		local_device_context->IASetIndexBuffer( indexBuffer, INDEX_BUFFER_FORMAT, offset );
	}
}


/**@brief Ustawia w kontekœcie urz¹dzenia bufor wierzcho³ków.

@param[in] buffer Bufor do ustawienia.
@return Je¿eli bufor nie istnieje to zwraca wartoœæ true. Inaczej false.
Wywo³anie if( set_vertex_buffer() ) ma zwróciæ tak¹ wartoœæ, ¿eby w ifie mo¿na by³o
wywo³aæ return lub continue, w przypadku braku bufora.*/
inline bool DX11Renderer::setVertexBuffer( BufferObject* buffer )
{
	ID3D11Buffer* vertexBuffer = nullptr;
	if ( buffer )
	{
		vertexBuffer = buffer->get();
		unsigned int stride = buffer->get_stride();
		unsigned int offset = 0;
		local_device_context->IASetVertexBuffers( 0, 1, &vertexBuffer, &stride, &offset );

		return false;
	}
	return true;
}



inline void DX11Renderer::depthBufferEnable( bool state )
{

}



