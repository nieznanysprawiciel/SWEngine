#pragma once
#include "DX11Renderer.h"


/**@brief Klasa b�dzie renderowa� w trybie immediate albo deferred.

Je�eli ustawiono flag� D3D11_CREATE_DEVICE_SINGLETHREADED, to nie wolno poda� w parametrze
RENDERER_USAGE::USE_AS_DEFERRED.

Mo�na sprawdzi� czy klasa zosta�a zainicjowana poprawnie wywo�uj�c funkcj� isValid.*/
inline DX11Renderer::DX11Renderer( RENDERER_USAGE usage )
{

	if ( usage == RENDERER_USAGE::USE_AS_IMMEDIATE )
	{
		m_localDeviceContext = device_context;
		m_valid = true;
	}
	else if ( usage == RENDERER_USAGE::USE_AS_DEFERRED )		// Will not work, if the device was created with the D3D11_CREATE_DEVICE_SINGLETHREADED value.
	{
		device->CreateDeferredContext( 0, &m_localDeviceContext );
		m_valid = true;
	}
	else
	{
		m_valid = false;
		assert( false );
	}
}


inline DX11Renderer::~DX11Renderer()
{
	if ( m_usageType == RENDERER_USAGE::USE_AS_DEFERRED )	/// Tylko w takim wypadku alokowali�my nowy kontekst. Inaczej zwalanianie nale�y do klasy DX11_interfaces_container.
		if ( m_localDeviceContext )
			m_localDeviceContext->Release();
}


/**@brief Funkcja ustawia tesktury z danego ModelParta w DirectXie.

@param[in] model ModelPart z kt�rego pochodz� tekstury do ustawienia.
@todo SetShaderResource mo�na u�y� do ustawienia od razu ca�ej tablicy. Trzeba umo�liwi� ustawianie
do VS i innych.*/
inline void DX11Renderer::SetTextures( const ModelPart& model )
{
	for ( int i = 0; i < ENGINE_MAX_TEXTURES; ++i )
		if ( model.texture[i] )		// Nie ka�da tekstura w tablicy istnieje
		{
			ID3D11ShaderResourceView* tex = model.texture[i]->get();
			m_localDeviceContext->PSSetShaderResources( i, 1, &tex );
		}
}

/**@brief Ustawia w kontek�cie urz�dzenia bufor indeks�w.

@param[in] buffer Bufor do ustawienia.*/
inline void DX11Renderer::SetIndexBuffer( BufferObject* buffer )
{
	// Ustawiamy bufor indeks�w, je�eli istnieje
	ID3D11Buffer* indexBuffer = nullptr;
	if ( buffer )
	{
		indexBuffer = buffer->get();
		unsigned int offset = 0;
		m_localDeviceContext->IASetIndexBuffer( indexBuffer, INDEX_BUFFER_FORMAT, offset );
	}
}


/**@brief Ustawia w kontek�cie urz�dzenia bufor wierzcho�k�w.

@param[in] buffer Bufor do ustawienia.
@return Je�eli bufor nie istnieje to zwraca warto�� true. Inaczej false.
Wywo�anie if( set_vertex_buffer() ) ma zwr�ci� tak� warto��, �eby w ifie mo�na by�o
wywo�a� return lub continue, w przypadku braku bufora.*/
inline bool DX11Renderer::SetVertexBuffer( BufferObject* buffer, unsigned int offset )
{
	ID3D11Buffer* vertexBuffer = nullptr;
	if ( buffer )
	{
		vertexBuffer = buffer->get();
		unsigned int stride = buffer->get_stride();
		m_localDeviceContext->IASetVertexBuffers( 0, 1, &vertexBuffer, &stride, &offset );

		return false;
	}
	return true;
}


/**@brief Funkcja w��cza lub wy��cza z-bufor.

@param[in] state True je�eli z-bufor ma by� w��czony, false je�eli wy��czony.*/
inline void DX11Renderer::DepthBufferEnable( bool state )
{
	if ( state )
		m_localDeviceContext->OMSetDepthStencilState( depth_enabled, 1 );
	else
		m_localDeviceContext->OMSetDepthStencilState( depth_disabled, 1 );
}

/**@brief Ustawia domy�lny layout z DX11_interfaces_container.

@todo Wymy�le� lepsz� implementacj� i spos�b przechowywania layout�w.*/
inline void DX11Renderer::SetDefaultVertexLayout()
{
	m_localDeviceContext->IASetInputLayout( default_vertex_layout );
}

/**@brief Ustawia domy�lny sampler z DX11_interfaces_container.

@todo Wymy�le� lepsz� implementacj� i spos�b przechowywania sampler�w. (W zasadzie mo�na je definiowa� w shaderach.)*/
inline void DX11Renderer::SetDefaultSampler()
{
	m_localDeviceContext->PSSetSamplers( 0, 1, &default_sampler );
}

/**@brief Inicjuje bufory sta�ych dla shader�w.

@todo: Ta funkcja powinna znikn��. Bufory powinny by� inicjowane w DisplayEngine, ale jako BufferObject,
a nie bufory DirectXowe.*/
inline void DX11Renderer::InitBuffers( unsigned int sizePerFrame, unsigned int sizePerMesh )
{
	init_buffers( sizePerFrame, sizePerMesh );
}

/**@brief Inicjuje pomocnicze stany bufora g��boko�ci (do w��czania i wy��czania depth testu)

@todo To trzeba za�atwia� w jaki� bardziej elegancki spos�b.*/
inline void DX11Renderer::InitDepthStates()
{
	init_depth_states();
}

//----------------------------------------------------------------------------------------------//
//								Funkcje analogiczne do DirectX11								//
//----------------------------------------------------------------------------------------------//

/**@brief Funkcja robi dok�adnie to samo, co tak samo nazywaj�ca si� funkcja DirectXa.
Po wi�cej informacji wysy�am do MSDNu.

@todo Funkcja nie powinna przyjmowa� w parametrze sta�ej definiowanej w DirectX.
Nale�y zrobi� w�asny zestaw sta�ych, �eby uniezale�ni� interfejs renderera od implementacji.*/
inline void DX11Renderer::IASetPrimitiveTopology( D3D_PRIMITIVE_TOPOLOGY topology )
{
	m_localDeviceContext->IASetPrimitiveTopology( topology );
}

/**@brief Funkcja robi dok�adnie to samo, co tak samo nazywaj�ca si� funkcja DirectXa.
Po wi�cej informacji wysy�am do MSDNu.

@todo Funkcja nie powinna przyjmowa� w parametrze obiektu DirectXa.
Trzeba co� z tym zrobi�. Nie wiem co*/
inline void DX11Renderer::IASetInputLayout( ID3D11InputLayout *pInputLayout )
{
	m_localDeviceContext->IASetInputLayout( pInputLayout );
}

/**@brief Funkcja robi dok�adnie to samo, co tak samo nazywaj�ca si� funkcja DirectXa.
Po wi�cej informacji wysy�am do MSDNu.

@deprecated Powinni�my u�y� bufora silnikowego, a nie DirectXowego, �eby ukry� implementacj�.
Funkcja zniknie w p�niejszej wersji.*/
inline void DX11Renderer::VSSetConstantBuffers( UINT startSlot, UINT numBuffers, ID3D11Buffer *const *ppConstantBuffers )
{
	m_localDeviceContext->VSSetConstantBuffers( startSlot, numBuffers, ppConstantBuffers );
}

/**@brief Funkcja robi dok�adnie to samo, co tak samo nazywaj�ca si� funkcja DirectXa.
Po wi�cej informacji wysy�am do MSDNu.

@deprecated Powinni�my u�y� bufora silnikowego, a nie DirectXowego, �eby ukry� implementacj�.
Funkcja zniknie w p�niejszej wersji.*/
inline void DX11Renderer::PSSetConstantBuffers( UINT startSlot, UINT numBuffers, ID3D11Buffer *const *ppConstantBuffers )
{
	m_localDeviceContext->PSSetConstantBuffers( startSlot, numBuffers, ppConstantBuffers );
}

/**@brief Funkcja robi dok�adnie to samo, co tak samo nazywaj�ca si� funkcja DirectXa.
Po wi�cej informacji wysy�am do MSDNu.*/
inline void DX11Renderer::VSSetConstantBuffers( UINT startSlot, UINT numBuffers, BufferObject* buffer )
{
	ID3D11Buffer* directXBuffer = buffer->get();
	m_localDeviceContext->VSSetConstantBuffers( startSlot, numBuffers, &directXBuffer );
}

/**@brief Funkcja robi dok�adnie to samo, co tak samo nazywaj�ca si� funkcja DirectXa.
Po wi�cej informacji wysy�am do MSDNu.*/
inline void DX11Renderer::PSSetConstantBuffers( UINT startSlot, UINT numBuffers, BufferObject* buffer )
{
	ID3D11Buffer* directXBuffer = buffer->get();
	m_localDeviceContext->PSSetConstantBuffers( startSlot, numBuffers, &directXBuffer );
}

/**@brief Funkcja robi dok�adnie to samo, co tak samo nazywaj�ca si� funkcja DirectXa.
Po wi�cej informacji wysy�am do MSDNu.

@deprecated: Depracated jeszcze przed napisaniem funkcji do ko�ca. My�l�, �e bufor powinien
sam si� updatowa�.*/
inline void DX11Renderer::UpdateSubresource( ID3D11Resource *pDstResource, const void *pSrcData )
{
	m_localDeviceContext->UpdateSubresource( pDstResource, 0, nullptr, pSrcData, 0, 0 );
}

inline void DX11Renderer::SetShaders( ModelPart& model )
{
	m_localDeviceContext->VSSetShader( model.vertex_shader->get(), nullptr, 0 );
	m_localDeviceContext->PSSetShader( model.pixel_shader->get(), nullptr, 0 );
}

//----------------------------------------------------------------------------------------------//
//								Rendering														//
//----------------------------------------------------------------------------------------------//

/**@brief Funkcja robi dok�adnie to samo, co tak samo nazywaj�ca si� funkcja DirectXa.
Po wi�cej informacji wysy�am do MSDNu.*/
inline void DX11Renderer::Draw( unsigned int vertexCount, unsigned int startVertexLocation )
{
	m_localDeviceContext->Draw( vertexCount, startVertexLocation );
}

/**@brief Funkcja robi dok�adnie to samo, co tak samo nazywaj�ca si� funkcja DirectXa.
Po wi�cej informacji wysy�am do MSDNu.*/
inline void DX11Renderer::DrawIndexed( unsigned int indexCount, unsigned int startIndexLocation, int baseVertexLocation )
{
	m_localDeviceContext->DrawIndexed( indexCount, startIndexLocation, baseVertexLocation );
}
