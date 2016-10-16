/**
@file DX11Renderer.cpp
@author nieznanysprawiciel
@copyright File is part of graphic engine SWEngine.
*/
#include "stdafx.h"


#include "DX11Renderer.h"

#include "DX11Initializer/DX11ConstantsMapper.h"
#include "DX11Resources/DX11InputLayout.h"
#include "DX11Resources/DX11Buffer.h"
#include "DX11Resources/DX11VertexShader.h"
#include "DX11Resources/DX11PixelShader.h"
#include "DX11Resources/DX11Texture.h"
#include "DX11Resources/DX11RenderTarget.h"

#include "Common/MemoryLeaks.h"


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
void	DX11Renderer::SetTextures( const ModelPart& model )
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
void	DX11Renderer::SetIndexBuffer( BufferObject* buffer )
{
	// Ustawiamy bufor indeksów, je¿eli istnieje
	ID3D11Buffer* indexBuffer = nullptr;
	if ( buffer )
	{
		indexBuffer = DX11( buffer )->Get();
		unsigned int offset = 0;
		m_localDeviceContext->IASetIndexBuffer( indexBuffer, INDEX_BUFFER_FORMAT, offset );
	}
}


/**@brief Ustawia w kontekœcie urz¹dzenia bufor wierzcho³ków.

@param[in] buffer Bufor do ustawienia.
@return Je¿eli bufor nie istnieje to zwraca wartoœæ true. Inaczej false.
Wywo³anie if( SetVertexBuffer() ) ma zwróciæ tak¹ wartoœæ, ¿eby w ifie mo¿na by³o
wywo³aæ return lub continue, w przypadku braku bufora.*/
bool DX11Renderer::SetVertexBuffer( BufferObject* buffer, unsigned int offset )
{
	ID3D11Buffer* vertexBuffer = nullptr;
	if ( buffer )
	{
		vertexBuffer = DX11( buffer )->Get();
		unsigned int stride = buffer->GetStride();
		m_localDeviceContext->IASetVertexBuffers( 0, 1, &vertexBuffer, &stride, &offset );

		return false;
	}
	else
	{
		//throw new std::runtime_error( "Vertex buffer is nullptr" );
		assert( !"Vertex buffer is nullptr" );
	}
	return true;
}


/**@brief Funkcja w³¹cza lub wy³¹cza z-bufor.

@param[in] state True je¿eli z-bufor ma byæ w³¹czony, false je¿eli wy³¹czony.*/
void	DX11Renderer::DepthBufferEnable( bool state )
{
	if ( state )
		m_localDeviceContext->OMSetDepthStencilState( depth_enabled, 1 );
	else
		m_localDeviceContext->OMSetDepthStencilState( depth_disabled, 1 );
}

/**@brief Ustawia domyœlny layout z DX11_interfaces_container.

@todo Wymyœleæ lepsz¹ implementacjê i sposób przechowywania layoutów.*/
void	DX11Renderer::SetDefaultVertexLayout()
{
	m_localDeviceContext->IASetInputLayout( default_vertex_layout );
}

/**@brief Ustawia domyœlny sampler z DX11_interfaces_container.

@todo Wymyœleæ lepsz¹ implementacjê i sposób przechowywania samplerów. (W zasadzie mo¿na je definiowaæ w shaderach.)*/
void	DX11Renderer::SetDefaultSampler()
{
	m_localDeviceContext->PSSetSamplers( 0, 1, &default_sampler );
}

/**@brief Inicjuje bufory sta³ych dla shaderów.

@todo: Ta funkcja powinna znikn¹æ. Bufory powinny byæ inicjowane w DisplayEngine, ale jako BufferObject,
a nie bufory DirectXowe.*/
void	DX11Renderer::InitBuffers( unsigned int sizePerFrame, unsigned int sizePerMesh )
{
	init_buffers( sizePerFrame, sizePerMesh );
}

/**@brief Inicjuje pomocnicze stany bufora g³êbokoœci (do w³¹czania i wy³¹czania depth testu)

@todo To trzeba za³atwiaæ w jakiœ bardziej elegancki sposób.*/
void	DX11Renderer::InitDepthStates()
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
void	DX11Renderer::IASetPrimitiveTopology( PrimitiveTopology topology )
{
	m_localDeviceContext->IASetPrimitiveTopology( DX11ConstantsMapper::Get( topology ) );
}

/**@brief Funkcja robi dok³adnie to samo, co tak samo nazywaj¹ca siê funkcja DirectXa.
Po wiêcej informacji wysy³am do MSDNu.*/
void	DX11Renderer::IASetInputLayout( ShaderInputLayout* inputLayout )
{
	m_localDeviceContext->IASetInputLayout( static_cast<DX11InputLayout*>( inputLayout )->Get() );
}

///**@brief Funkcja robi dok³adnie to samo, co tak samo nazywaj¹ca siê funkcja DirectXa.
//Po wiêcej informacji wysy³am do MSDNu.
//
//@deprecated Powinniœmy u¿yæ bufora silnikowego, a nie DirectXowego, ¿eby ukryæ implementacjê.
//Funkcja zniknie w póŸniejszej wersji.*/
//void	DX11Renderer::VSSetConstantBuffers( uint32 startSlot, uint32 numBuffers, ID3D11Buffer *const *ppConstantBuffers )
//{
//	m_localDeviceContext->VSSetConstantBuffers( startSlot, numBuffers, ppConstantBuffers );
//}
//
///**@brief Funkcja robi dok³adnie to samo, co tak samo nazywaj¹ca siê funkcja DirectXa.
//Po wiêcej informacji wysy³am do MSDNu.
//
//@deprecated Powinniœmy u¿yæ bufora silnikowego, a nie DirectXowego, ¿eby ukryæ implementacjê.
//Funkcja zniknie w póŸniejszej wersji.*/
//void	DX11Renderer::PSSetConstantBuffers( uint32 startSlot, uint32 numBuffers, ID3D11Buffer *const *ppConstantBuffers )
//{
//	m_localDeviceContext->PSSetConstantBuffers( startSlot, numBuffers, ppConstantBuffers );
//}

/**@brief Funkcja robi dok³adnie to samo, co tak samo nazywaj¹ca siê funkcja DirectXa.
Po wiêcej informacji wysy³am do MSDNu.*/
void	DX11Renderer::VSSetConstantBuffers( uint32 slot, BufferObject* buffer )
{
	ID3D11Buffer* directXBuffer = DX11(buffer)->Get();
	m_localDeviceContext->VSSetConstantBuffers( slot, 1, &directXBuffer );
}

/**@brief Funkcja robi dok³adnie to samo, co tak samo nazywaj¹ca siê funkcja DirectXa.
Po wiêcej informacji wysy³am do MSDNu.*/
void	DX11Renderer::PSSetConstantBuffers( uint32 slot, BufferObject* buffer )
{
	ID3D11Buffer* directXBuffer = DX11(buffer)->Get();
	m_localDeviceContext->PSSetConstantBuffers( slot, 1, &directXBuffer );
}

/**@brief Funkcja robi dok³adnie to samo, co tak samo nazywaj¹ca siê funkcja DirectXa.
Po wiêcej informacji wysy³am do MSDNu.

@deprecated: Depracated jeszcze przed napisaniem funkcji do koñca. Myœlê, ¿e bufor powinien
sam siê updatowaæ.*/
void	DX11Renderer::UpdateSubresource( BufferObject* pDstResource, const void *pSrcData )
{
	m_localDeviceContext->UpdateSubresource( static_cast<DX11Buffer*>( pDstResource)->Get(), 0, nullptr, pSrcData, 0, 0 );
}

void	DX11Renderer::SetShaders( ModelPart& model )
{
	m_localDeviceContext->VSSetShader( static_cast<DX11VertexShader*>( model.vertex_shader )->Get(), nullptr, 0 );
	m_localDeviceContext->PSSetShader( static_cast<DX11PixelShader*>( model.pixel_shader )->Get(), nullptr, 0 );
}

//----------------------------------------------------------------------------------------------//
//								Rendering														//
//----------------------------------------------------------------------------------------------//

/**@brief Funkcja robi dok³adnie to samo, co tak samo nazywaj¹ca siê funkcja DirectXa.
Po wiêcej informacji wysy³am do MSDNu.*/
void	DX11Renderer::Draw( unsigned int vertexCount, unsigned int startVertexLocation )
{
	m_localDeviceContext->Draw( vertexCount, startVertexLocation );
}

/**@brief Funkcja robi dok³adnie to samo, co tak samo nazywaj¹ca siê funkcja DirectXa.
Po wiêcej informacji wysy³am do MSDNu.*/
void	DX11Renderer::DrawIndexed( unsigned int indexCount, unsigned int startIndexLocation, int baseVertexLocation )
{
	m_localDeviceContext->DrawIndexed( indexCount, startIndexLocation, baseVertexLocation );
}

/**@brief Wykonywane przed rozpoczêciem renderowania.

@todo Polepszyæ, poprawiæ zmieniæ.*/
void	DX11Renderer::Present()
{
	swap_chain->Present( 0, 0 );
}

/**@brief Wyœwietla renderowan¹ scenê.

@todo Polepszyæ, poprawiæ zmieniæ. Powinno obs³ugiwaæ renderowanie natychmiastowe i synchronizacjê poziom¹ w parametrze.*/
void	DX11Renderer::BeginScene( RenderTargetObject* mainRenderTarget )
{
	DX11RenderTarget* renderTarget = DX11( mainRenderTarget );
	auto renderTargetView = renderTarget->GetRenderTarget();
	auto depthStencilView = renderTarget->GetDepthStencil();

	// Czyszczenie render targetów
	float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };	// red, green, blue, alpha
	device_context->ClearRenderTargetView( renderTargetView, ClearColor );
	device_context->ClearDepthStencilView( depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0 );

	// Ustawienie aktualnego render targetu
	///@todo Render target powinien byæ ustawiany gdzie indziej. To jest tymczasowe rozwi¹zanie.
	device_context->OMSetRenderTargets( 1, &renderTargetView, depthStencilView );
	
	auto viewport = get_viewport_desc();
	viewport.Width = renderTarget->GetWidth();
	viewport.Height = renderTarget->GetHeight();
	device_context->RSSetViewports( 1, &viewport );
}

//====================================================================================//
//			Drawing commands
//====================================================================================//

// ================================ //
//
void	DX11Renderer::Draw				( const DrawCommand& command )
{
	auto indexBuffer = DX11( command.IndexBufer )->Get();
	auto layout = DX11( command.Layout )->Get();

	m_localDeviceContext->IASetPrimitiveTopology( DX11ConstantsMapper::Get( command.Topology ) );
	m_localDeviceContext->IASetInputLayout( layout );

	DX11Renderer::SetVertexBuffer( command.VertexBuffer, 0 );
	SetIndexBuffer( command.IndexBufer, 0, command.ExtendedIndex );

	if( indexBuffer )
		m_localDeviceContext->DrawIndexed( command.NumVertices, command.BufferOffset, command.BaseVertex );
	else
		m_localDeviceContext->Draw( command.NumVertices, command.BufferOffset );
}

// ================================ //
//
void	DX11Renderer::DrawInstanced		( const DrawInstancedCommand& command )
{
	auto indexBuffer = DX11( command.IndexBufer )->Get();
	auto layout = DX11( command.Layout )->Get();

	m_localDeviceContext->IASetPrimitiveTopology( DX11ConstantsMapper::Get( command.Topology ) );
	m_localDeviceContext->IASetInputLayout( layout );

	SetIndexBuffer( command.IndexBufer, 0, command.ExtendedIndex );

	ID3D11Buffer* inputBuffer[ 2 ];
	inputBuffer[ 0 ] = DX11( command.VertexBuffer )->Get();
	inputBuffer[ 1 ] = DX11( command.PerInstanceBuffer )->Get();

	uint32 strides[ 2 ] = { command.VertexBuffer->GetStride(), command.PerInstanceBuffer->GetStride() };
	uint32 offsets[ 2 ] = { 0, 0 };

	m_localDeviceContext->IASetVertexBuffers( 0, 2, inputBuffer, strides, offsets );

	if( indexBuffer )
		m_localDeviceContext->DrawIndexedInstanced( command.NumVertices, command.NumInstances, command.BufferOffset, command.BaseVertex, 0 );
	else
		m_localDeviceContext->DrawInstanced( command.NumVertices, command.NumInstances, command.BufferOffset, 0 );
}

//====================================================================================//
//			Render target commands	
//====================================================================================//

// ================================ //
//
void	DX11Renderer::SetRenderTarget	( const SetRenderTargetCommand& command )
{
	SetRenderTarget( command.RenderTargets, command.DepthStencil );

	assert( command.RenderTargets[ 0 ] );
	assert( !"implement me" );

	DX11RenderTarget* renderTarget = DX11( command.RenderTargets[ 0 ] );

	D3D11_VIEWPORT viewport;
	viewport.Width = renderTarget->GetWidth();
	viewport.Height = renderTarget->GetHeight();
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;

	device_context->RSSetViewports( 1, &viewport );
	device_context->RSSetScissorRects( 0, nullptr );

	float blendFactor[ 4 ] = { 0, 0, 0, 0 };

	device_context->RSSetState( DX11( command.RasterizerState )->Get() );
	device_context->OMSetBlendState( DX11( command.BlendingState )->Get(), blendFactor, 0xffffffff );
	device_context->OMSetDepthStencilState( DX11( command.DepthStencilState )->Get(), 0 );

	auto cameraBuffer = DX11( command.CameraBuffer );
	auto lightBuffer = DX11( command.LightBuffer );

	if( cameraBuffer )
	{
		ID3D11Buffer* directXBuffer = cameraBuffer->Get();
		m_localDeviceContext->VSSetConstantBuffers( CAMERA_BUFFER_BINDING_POINT, 1, &directXBuffer );
		m_localDeviceContext->PSGetConstantBuffers( CAMERA_BUFFER_BINDING_POINT, 1, &directXBuffer );
	}

	if( lightBuffer )
	{
		ID3D11Buffer* directXBuffer = lightBuffer->Get();
		m_localDeviceContext->PSGetConstantBuffers( CAMERA_BUFFER_BINDING_POINT, 1, &directXBuffer );
	}
}

// ================================ //
//
void	DX11Renderer::SetRenderTarget	( const SetRenderTargetExCommand& command )
{
	assert( !"implement me" );

	SetRenderTarget( command.RenderTargets, command.DepthStencil );

	if( command.NumViews > MAX_BOUND_RENDER_TARGETS )
	{
		assert( false );
		//throw new std::runtime_error( "Trying bind to many viewports or scissor rectangles." );
	}

	D3D11_VIEWPORT viewport[ MAX_BOUND_RENDER_TARGETS ];
	D3D11_RECT scissors[ MAX_BOUND_RENDER_TARGETS ];

	for( int i = 0; i < command.NumViews; ++i )
	{
		viewport[ i ].Width = command.Viewports[ i ].Width;
		viewport[ i ].Height = command.Viewports[ i ].Height;
		viewport[ i ].MinDepth = command.Viewports[ i ].MinDepth;
		viewport[ i ].MaxDepth = command.Viewports[ i ].MaxDepth;
		viewport[ i ].TopLeftX = command.Viewports[ i ].TopLeftX;
		viewport[ i ].TopLeftY = command.Viewports[ i ].TopLeftY;

		scissors[ i ].top = command.Scissors[ i ].Top;
		scissors[ i ].bottom = command.Scissors[ i ].Bottom;
		scissors[ i ].left = command.Scissors[ i ].Left;
		scissors[ i ].right = command.Scissors[ i ].Right;
	}

	device_context->RSSetViewports( command.NumViews, viewport );
	device_context->RSSetScissorRects( command.NumViews, scissors );

	float blendFactor[ 4 ] = { command.BlendFactor.x, command.BlendFactor.y, command.BlendFactor.z, command.BlendFactor.w };

	device_context->RSSetState( DX11( command.RasterizerState )->Get() );
	device_context->OMSetBlendState( DX11( command.BlendingState )->Get(), blendFactor, command.StencilMask );
	device_context->OMSetDepthStencilState( DX11( command.DepthStencilState )->Get(), 0 );

	auto cameraBuffer = DX11( command.CameraBuffer );
	auto lightBuffer = DX11( command.LightBuffer );

	if( cameraBuffer )
	{
		ID3D11Buffer* directXBuffer = cameraBuffer->Get();
		m_localDeviceContext->VSSetConstantBuffers( CAMERA_BUFFER_BINDING_POINT, 1, &directXBuffer );
		m_localDeviceContext->PSGetConstantBuffers( CAMERA_BUFFER_BINDING_POINT, 1, &directXBuffer );
	}

	if( lightBuffer )
	{
		ID3D11Buffer* directXBuffer = lightBuffer->Get();
		m_localDeviceContext->PSGetConstantBuffers( CAMERA_BUFFER_BINDING_POINT, 1, &directXBuffer );
	}
}

// ================================ //
//
void	DX11Renderer::ClearRenderTarget	( const ClearRenderTargetCommand& command )
{ }

// ================================ //
//
void	DX11Renderer::UpdateBuffer		( const UpdateBufferCommand& command )
{ }

// ================================ //
//
void	DX11Renderer::BindBuffer		( const BindBufferCommand& command )
{ }

// ================================ //
//
void	DX11Renderer::UpdateAndBindBuffer	( const UpdateBindBuffer& command )
{ }

// ================================ //
//
void	DX11Renderer::SetDefaultBuffers	( const SetDefaultBuffersCommand& command )
{ }

// ================================ //
//
void	DX11Renderer::SetShaderState	( const SetShaderStateCommand& command )
{ }

// ================================ //
//
void	DX11Renderer::SetShaderState	( const SetShaderStateExCommand& command )
{ }

// ================================ //
//
void	DX11Renderer::SetShaderState	( const SetRenderStateCommand& command )
{ }

// ================================ //
//
void	DX11Renderer::SetShaderState	( const SetRenderStateExCommand& command )
{ }

// ================================ //
//
void	DX11Renderer::CopyTexture		( const CopyTextureCommand& command )
{ }


//====================================================================================//
//			Internal Helpers	
//====================================================================================//

// ================================ //
// Buffer can be nullptr.
void	DX11Renderer::SetIndexBuffer	( BufferObject* buffer, unsigned int offset, bool extendedIndex )
{
	if ( buffer )
	{
		auto indexBuffer = DX11( buffer )->Get();
		assert( indexBuffer );

		unsigned int offset = 0;
		auto indexFormat = extendedIndex ? DXGI_FORMAT_R32_UINT : DXGI_FORMAT_R16_UINT;

		m_localDeviceContext->IASetIndexBuffer( indexBuffer, indexFormat, offset );
	}
}

// ================================ //
//
void	DX11Renderer::SetRenderTarget	( RenderTargetObject* const targets[ MAX_BOUND_RENDER_TARGETS ], RenderTargetObject* depthStencil )
{
	ID3D11RenderTargetView* DX11Targets[ MAX_BOUND_RENDER_TARGETS ];
	for( int i = 0; i < MAX_BOUND_RENDER_TARGETS; ++i )
	{
		auto renderTargetObj = DX11( targets[ i ] );
		if( renderTargetObj )
			DX11Targets[ i ] = DX11( targets[ i ] )->GetRenderTarget();
		else
			DX11Targets[ i ] = nullptr;
	}

	auto depthStencilBuffer = DX11( depthStencil );
	ID3D11DepthStencilView* depthStencilView = nullptr;
	if( depthStencilBuffer )
		depthStencilView = depthStencilBuffer->GetDepthStencil();

	assert( !"implement me" );

	device_context->OMSetRenderTargets( MAX_BOUND_RENDER_TARGETS, DX11Targets, depthStencilView );
}

