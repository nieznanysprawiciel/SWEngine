#include "DX11Renderer.h"

#include "../DX11Initializer/DX11ConstantsMapper.h"
#include "../DX11Resources/DX11InputLayout.h"
#include "../DX11Resources/DX11Buffer.h"
#include "../DX11Resources/DX11VertexShader.h"
#include "../DX11Resources/DX11PixelShader.h"
#include "../DX11Resources/DX11Texture.h"
#include "../DX11Resources/DX11RenderTarget.h"

#include "Common/memory_leaks.h"


#ifdef INDEX_BUFFER_UINT16
	#define INDEX_BUFFER_FORMAT DXGI_FORMAT_R16_UINT		///<Format bufora indeksów. @todo W ostatecznej wersji aplikacji format mo¿na bêdzie wybieraæ.
#endif // INDEX_BUFFER_UINT16

#ifdef INDEX_BUFFER_UINT32
	#define INDEX_BUFFER_FORMAT DXGI_FORMAT_R32_UINT		///<Format bufora indeksów. @todo W ostatecznej wersji aplikacji format mo¿na bêdzie wybieraæ.
#endif // INDEX_BUFFER_UINT32



/**@brief Klasa bêdzie renderowaæ w trybie immediate albo deferred.

Je¿eli ustawiono flagê D3D11_CREATE_DEVICE_SINGLETHREADED, to nie wolno podaæ w parametrze
RendererUsage::USE_AS_DEFERRED.

Mo¿na sprawdziæ czy klasa zosta³a zainicjowana poprawnie wywo³uj¹c funkcjê isValid.*/
DX11Renderer::DX11Renderer( RendererUsage usage )
{
	m_usageType = usage;
	if ( usage == RendererUsage::USE_AS_IMMEDIATE )
	{
		m_localDeviceContext = device_context;
		m_valid = true;
	}
	else if ( usage == RendererUsage::USE_AS_DEFERRED )		// Will not work, if the device was created with the D3D11_CREATE_DEVICE_SINGLETHREADED value.
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


DX11Renderer::~DX11Renderer()
{
	if ( m_usageType == RendererUsage::USE_AS_DEFERRED )	/// Tylko w takim wypadku alokowaliœmy nowy kontekst. Inaczej zwalanianie nale¿y do klasy DX11_interfaces_container.
		if ( m_localDeviceContext )
			m_localDeviceContext->Release();
}


/**@brief Funkcja ustawia tesktury z danego ModelParta w DirectXie.

@param[in] model ModelPart z którego pochodz¹ tekstury do ustawienia.
@todo SetShaderResource mo¿na u¿yæ do ustawienia od razu ca³ej tablicy. Trzeba umo¿liwiæ ustawianie
do VS i innych.*/
void DX11Renderer::SetTextures( const ModelPart& model )
{
	for ( int i = 0; i < ENGINE_MAX_TEXTURES; ++i )
		if ( model.texture[i] )		// Nie ka¿da tekstura w tablicy istnieje
		{
			ID3D11ShaderResourceView* tex = static_cast<DX11Texture*>( model.texture[i] )->Get();
			m_localDeviceContext->PSSetShaderResources( i, 1, &tex );
		}
}

/**@brief Ustawia w kontekœcie urz¹dzenia bufor indeksów.

@param[in] buffer Bufor do ustawienia.*/
void DX11Renderer::SetIndexBuffer( BufferObject* buffer )
{
	// Ustawiamy bufor indeksów, je¿eli istnieje
	ID3D11Buffer* indexBuffer = nullptr;
	if ( buffer )
	{
		indexBuffer = static_cast<DX11Buffer*>( buffer )->Get();
		unsigned int offset = 0;
		m_localDeviceContext->IASetIndexBuffer( indexBuffer, INDEX_BUFFER_FORMAT, offset );
	}
}


/**@brief Ustawia w kontekœcie urz¹dzenia bufor wierzcho³ków.

@param[in] buffer Bufor do ustawienia.
@return Je¿eli bufor nie istnieje to zwraca wartoœæ true. Inaczej false.
Wywo³anie if( set_vertex_buffer() ) ma zwróciæ tak¹ wartoœæ, ¿eby w ifie mo¿na by³o
wywo³aæ return lub continue, w przypadku braku bufora.*/
bool DX11Renderer::SetVertexBuffer( BufferObject* buffer, unsigned int offset )
{
	ID3D11Buffer* vertexBuffer = nullptr;
	if ( buffer )
	{
		vertexBuffer = static_cast<DX11Buffer*>( buffer )->Get();
		unsigned int stride = buffer->GetStride();
		m_localDeviceContext->IASetVertexBuffers( 0, 1, &vertexBuffer, &stride, &offset );

		return false;
	}
	return true;
}


/**@brief Funkcja w³¹cza lub wy³¹cza z-bufor.

@param[in] state True je¿eli z-bufor ma byæ w³¹czony, false je¿eli wy³¹czony.*/
void DX11Renderer::DepthBufferEnable( bool state )
{
	if ( state )
		m_localDeviceContext->OMSetDepthStencilState( depth_enabled, 1 );
	else
		m_localDeviceContext->OMSetDepthStencilState( depth_disabled, 1 );
}

/**@brief Ustawia domyœlny layout z DX11_interfaces_container.

@todo Wymyœleæ lepsz¹ implementacjê i sposób przechowywania layoutów.*/
void DX11Renderer::SetDefaultVertexLayout()
{
	m_localDeviceContext->IASetInputLayout( default_vertex_layout );
}

/**@brief Ustawia domyœlny sampler z DX11_interfaces_container.

@todo Wymyœleæ lepsz¹ implementacjê i sposób przechowywania samplerów. (W zasadzie mo¿na je definiowaæ w shaderach.)*/
void DX11Renderer::SetDefaultSampler()
{
	m_localDeviceContext->PSSetSamplers( 0, 1, &default_sampler );
}

/**@brief Inicjuje bufory sta³ych dla shaderów.

@todo: Ta funkcja powinna znikn¹æ. Bufory powinny byæ inicjowane w DisplayEngine, ale jako BufferObject,
a nie bufory DirectXowe.*/
void DX11Renderer::InitBuffers( unsigned int sizePerFrame, unsigned int sizePerMesh )
{
	init_buffers( sizePerFrame, sizePerMesh );
}

/**@brief Inicjuje pomocnicze stany bufora g³êbokoœci (do w³¹czania i wy³¹czania depth testu)

@todo To trzeba za³atwiaæ w jakiœ bardziej elegancki sposób.*/
void DX11Renderer::InitDepthStates()
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
void DX11Renderer::IASetPrimitiveTopology( PrimitiveTopology topology )
{
	m_localDeviceContext->IASetPrimitiveTopology( DX11ConstantsMapper::Get( topology ) );
}

/**@brief Funkcja robi dok³adnie to samo, co tak samo nazywaj¹ca siê funkcja DirectXa.
Po wiêcej informacji wysy³am do MSDNu.*/
void DX11Renderer::IASetInputLayout( ShaderInputLayoutObject* inputLayout )
{
	m_localDeviceContext->IASetInputLayout( static_cast<DX11InputLayout*>( inputLayout )->Get() );
}

///**@brief Funkcja robi dok³adnie to samo, co tak samo nazywaj¹ca siê funkcja DirectXa.
//Po wiêcej informacji wysy³am do MSDNu.
//
//@deprecated Powinniœmy u¿yæ bufora silnikowego, a nie DirectXowego, ¿eby ukryæ implementacjê.
//Funkcja zniknie w póŸniejszej wersji.*/
//void DX11Renderer::VSSetConstantBuffers( uint32 startSlot, uint32 numBuffers, ID3D11Buffer *const *ppConstantBuffers )
//{
//	m_localDeviceContext->VSSetConstantBuffers( startSlot, numBuffers, ppConstantBuffers );
//}
//
///**@brief Funkcja robi dok³adnie to samo, co tak samo nazywaj¹ca siê funkcja DirectXa.
//Po wiêcej informacji wysy³am do MSDNu.
//
//@deprecated Powinniœmy u¿yæ bufora silnikowego, a nie DirectXowego, ¿eby ukryæ implementacjê.
//Funkcja zniknie w póŸniejszej wersji.*/
//void DX11Renderer::PSSetConstantBuffers( uint32 startSlot, uint32 numBuffers, ID3D11Buffer *const *ppConstantBuffers )
//{
//	m_localDeviceContext->PSSetConstantBuffers( startSlot, numBuffers, ppConstantBuffers );
//}

/**@brief Funkcja robi dok³adnie to samo, co tak samo nazywaj¹ca siê funkcja DirectXa.
Po wiêcej informacji wysy³am do MSDNu.*/
void DX11Renderer::VSSetConstantBuffers( uint32 slot, BufferObject* buffer )
{
	ID3D11Buffer* directXBuffer = static_cast<DX11Buffer*>(buffer)->Get();
	m_localDeviceContext->VSSetConstantBuffers( slot, 1, &directXBuffer );
}

/**@brief Funkcja robi dok³adnie to samo, co tak samo nazywaj¹ca siê funkcja DirectXa.
Po wiêcej informacji wysy³am do MSDNu.*/
void DX11Renderer::PSSetConstantBuffers( uint32 slot, BufferObject* buffer )
{
	ID3D11Buffer* directXBuffer = static_cast<DX11Buffer*>(buffer)->Get();
	m_localDeviceContext->PSSetConstantBuffers( slot, 1, &directXBuffer );
}

/**@brief Funkcja robi dok³adnie to samo, co tak samo nazywaj¹ca siê funkcja DirectXa.
Po wiêcej informacji wysy³am do MSDNu.

@deprecated: Depracated jeszcze przed napisaniem funkcji do koñca. Myœlê, ¿e bufor powinien
sam siê updatowaæ.*/
void DX11Renderer::UpdateSubresource( BufferObject* pDstResource, const void *pSrcData )
{
	m_localDeviceContext->UpdateSubresource( static_cast<DX11Buffer*>( pDstResource)->Get(), 0, nullptr, pSrcData, 0, 0 );
}

void DX11Renderer::SetShaders( ModelPart& model )
{
	m_localDeviceContext->VSSetShader( static_cast<DX11VertexShader*>( model.vertex_shader )->Get(), nullptr, 0 );
	m_localDeviceContext->PSSetShader( static_cast<DX11PixelShader*>( model.pixel_shader )->Get(), nullptr, 0 );
}

//----------------------------------------------------------------------------------------------//
//								Rendering														//
//----------------------------------------------------------------------------------------------//

/**@brief Funkcja robi dok³adnie to samo, co tak samo nazywaj¹ca siê funkcja DirectXa.
Po wiêcej informacji wysy³am do MSDNu.*/
void DX11Renderer::Draw( unsigned int vertexCount, unsigned int startVertexLocation )
{
	m_localDeviceContext->Draw( vertexCount, startVertexLocation );
}

/**@brief Funkcja robi dok³adnie to samo, co tak samo nazywaj¹ca siê funkcja DirectXa.
Po wiêcej informacji wysy³am do MSDNu.*/
void DX11Renderer::DrawIndexed( unsigned int indexCount, unsigned int startIndexLocation, int baseVertexLocation )
{
	m_localDeviceContext->DrawIndexed( indexCount, startIndexLocation, baseVertexLocation );
}

/**@brief Wykonywane przed rozpoczêciem renderowania.

@todo Polepszyæ, poprawiæ zmieniæ.*/
void DX11Renderer::Present()
{
	end_scene_and_present();
}

/**@brief Wyœwietla renderowan¹ scenê.

@todo Polepszyæ, poprawiæ zmieniæ. Powinno obs³ugiwaæ renderowanie natychmiastowe i synchronizacjê poziom¹ w parametrze.*/
void DX11Renderer::BeginScene( RenderTargetObject* mainRenderTarget )
{
	DX11RenderTarget* renderTarget = static_cast<DX11RenderTarget*>( mainRenderTarget );
	//Bufor tylny
	float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };	// red, green, blue, alpha
	device_context->ClearRenderTargetView( renderTarget->GetRenderTarget(), ClearColor );

	//Z-bufor
	device_context->ClearDepthStencilView( renderTarget->GetDepthStencil(), D3D11_CLEAR_DEPTH, 1.0f, 0 );
}

