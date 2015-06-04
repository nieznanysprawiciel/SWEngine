#pragma once
#include "DX11Renderer.h"


/**@brief Klasa bêdzie renderowaæ w trybie immediate albo deferred.

Je¿eli ustawiono flagê D3D11_CREATE_DEVICE_SINGLETHREADED, to nie wolno podaæ w parametrze
RENDERER_USAGE::USE_AS_DEFERRED.

Mo¿na sprawdziæ czy klasa zosta³a zainicjowana poprawnie wywo³uj¹c funkcjê isValid.*/
inline DX11Renderer::DX11Renderer( RENDERER_USAGE usage )
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


inline DX11Renderer::~DX11Renderer()
{
	if ( usage_type == RENDERER_USAGE::USE_AS_DEFERRED )	/// Tylko w takim wypadku alokowaliœmy nowy kontekst. Inaczej zwalanianie nale¿y do klasy DX11_interfaces_container.
		if ( local_device_context )
			local_device_context->Release();
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
inline bool DX11Renderer::setVertexBuffer( BufferObject* buffer, unsigned int offset )
{
	ID3D11Buffer* vertexBuffer = nullptr;
	if ( buffer )
	{
		vertexBuffer = buffer->get();
		unsigned int stride = buffer->get_stride();
		local_device_context->IASetVertexBuffers( 0, 1, &vertexBuffer, &stride, &offset );

		return false;
	}
	return true;
}


/**@brief Funkcja w³¹cza lub wy³¹cza z-bufor.

@param[in] state True je¿eli z-bufor ma byæ w³¹czony, false je¿eli wy³¹czony.*/
inline void DX11Renderer::depthBufferEnable( bool state )
{
	if ( state )
		local_device_context->OMSetDepthStencilState( depth_enabled, 1 );
	else
		local_device_context->OMSetDepthStencilState( depth_disabled, 1 );
}

/**@brief Ustawia domyœlny layout z DX11_interfaces_container.

@todo Wymyœleæ lepsz¹ implementacjê i sposób przechowywania layoutów.*/
inline void DX11Renderer::setDefaultVertexLayout()
{
	local_device_context->IASetInputLayout( default_vertex_layout );
}

/**@brief Ustawia domyœlny sampler z DX11_interfaces_container.

@todo Wymyœleæ lepsz¹ implementacjê i sposób przechowywania samplerów. (W zasadzie mo¿na je definiowaæ w shaderach.)*/
inline void DX11Renderer::setDefaultSampler()
{
	local_device_context->PSSetSamplers( 0, 1, &default_sampler );
}

/**@brief Inicjuje bufory sta³ych dla shaderów.

@todo: Ta funkcja powinna znikn¹æ. Bufory powinny byæ inicjowane w DisplayEngine, ale jako BufferObject,
a nie bufory DirectXowe.*/
inline void DX11Renderer::initBuffers( unsigned int size_per_frame, unsigned int size_per_mesh )
{
	init_buffers( size_per_frame, size_per_mesh );
}

/**@brief Inicjuje pomocnicze stany bufora g³êbokoœci (do w³¹czania i wy³¹czania depth testu)

@todo To trzeba za³atwiaæ w jakiœ bardziej elegancki sposób.*/
inline void DX11Renderer::initDepthStates()
{
	init_depth_states();
}

//----------------------------------------------------------------------------------------------//
//								Funkcje analogiczne do DirectX11								//
//----------------------------------------------------------------------------------------------//

/**@brief Funkcja robi dok³adnie to samo, co tak samo nazywaj¹ca siê funkcja DirectXa.
Po wiêcej informacji wysy³am do MSDNu.

@todo Funkcja nie powinna przyjmowaæ w parametrze sta³ej definiowanej w DirectX.
Nale¿y zrobiæ w³asny zestaw sta³ych, ¿eby uniezale¿niæ interfejs renderera od implementacji.*/
inline void DX11Renderer::IASetPrimitiveTopology( D3D_PRIMITIVE_TOPOLOGY topology )
{
	local_device_context->IASetPrimitiveTopology( topology );
}

/**@brief Funkcja robi dok³adnie to samo, co tak samo nazywaj¹ca siê funkcja DirectXa.
Po wiêcej informacji wysy³am do MSDNu.

@todo Funkcja nie powinna przyjmowaæ w parametrze obiektu DirectXa.
Trzeba coœ z tym zrobiæ. Nie wiem co*/
inline void DX11Renderer::IASetInputLayout( ID3D11InputLayout *pInputLayout )
{
	local_device_context->IASetInputLayout( pInputLayout );
}

/**@brief Funkcja robi dok³adnie to samo, co tak samo nazywaj¹ca siê funkcja DirectXa.
Po wiêcej informacji wysy³am do MSDNu.

@deprecated Powinniœmy u¿yæ bufora silnikowego, a nie DirectXowego, ¿eby ukryæ implementacjê.
Funkcja zniknie w póŸniejszej wersji.*/
inline void DX11Renderer::VSSetConstantBuffers( UINT StartSlot, UINT NumBuffers, ID3D11Buffer *const *ppConstantBuffers )
{
	local_device_context->VSSetConstantBuffers( StartSlot, NumBuffers, ppConstantBuffers );
}

/**@brief Funkcja robi dok³adnie to samo, co tak samo nazywaj¹ca siê funkcja DirectXa.
Po wiêcej informacji wysy³am do MSDNu.

@deprecated Powinniœmy u¿yæ bufora silnikowego, a nie DirectXowego, ¿eby ukryæ implementacjê.
Funkcja zniknie w póŸniejszej wersji.*/
inline void DX11Renderer::PSSetConstantBuffers( UINT StartSlot, UINT NumBuffers, ID3D11Buffer *const *ppConstantBuffers )
{
	local_device_context->PSSetConstantBuffers( StartSlot, NumBuffers, ppConstantBuffers );
}

/**@brief Funkcja robi dok³adnie to samo, co tak samo nazywaj¹ca siê funkcja DirectXa.
Po wiêcej informacji wysy³am do MSDNu.*/
inline void DX11Renderer::VSSetConstantBuffers( UINT StartSlot, UINT NumBuffers, BufferObject* buffer )
{
	ID3D11Buffer* directXBuffer = buffer->get();
	local_device_context->VSSetConstantBuffers( StartSlot, NumBuffers, &directXBuffer );
}

/**@brief Funkcja robi dok³adnie to samo, co tak samo nazywaj¹ca siê funkcja DirectXa.
Po wiêcej informacji wysy³am do MSDNu.*/
inline void DX11Renderer::PSSetConstantBuffers( UINT StartSlot, UINT NumBuffers, BufferObject* buffer )
{
	ID3D11Buffer* directXBuffer = buffer->get();
	local_device_context->PSSetConstantBuffers( StartSlot, NumBuffers, &directXBuffer );
}

/**@brief Funkcja robi dok³adnie to samo, co tak samo nazywaj¹ca siê funkcja DirectXa.
Po wiêcej informacji wysy³am do MSDNu.

@deprecated: Depracated jeszcze przed napisaniem funkcji do koñca. Myœlê, ¿e bufor powinien
sam siê updatowaæ.*/
inline void DX11Renderer::UpdateSubresource( ID3D11Resource *pDstResource, const void *pSrcData )
{
	local_device_context->UpdateSubresource( pDstResource, 0, nullptr, pSrcData, 0, 0 );
}

inline void DX11Renderer::setShaders( ModelPart& model )
{
	local_device_context->VSSetShader( model.vertex_shader->get(), nullptr, 0 );
	local_device_context->PSSetShader( model.pixel_shader->get(), nullptr, 0 );
}

//----------------------------------------------------------------------------------------------//
//								Rendering														//
//----------------------------------------------------------------------------------------------//

/**@brief Funkcja robi dok³adnie to samo, co tak samo nazywaj¹ca siê funkcja DirectXa.
Po wiêcej informacji wysy³am do MSDNu.*/
inline void DX11Renderer::Draw( unsigned int VertexCount, unsigned int StartVertexLocation )
{
	local_device_context->Draw( VertexCount, StartVertexLocation );
}

/**@brief Funkcja robi dok³adnie to samo, co tak samo nazywaj¹ca siê funkcja DirectXa.
Po wiêcej informacji wysy³am do MSDNu.*/
inline void DX11Renderer::DrawIndexed( unsigned int IndexCount, unsigned int StartIndexLocation, int BaseVertexLocation )
{
	local_device_context->DrawIndexed( IndexCount, StartIndexLocation, BaseVertexLocation );
}

