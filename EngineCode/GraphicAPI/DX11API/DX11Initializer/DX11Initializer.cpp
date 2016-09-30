#include "DX11Initializer.h"
#include "../DX11Renderer/DX11Renderer.h"
#include "../DX11Resources/DX11RenderTarget.h"
#include "../DX11Resources/DX11Texture.h"
#include "DX11ConstantsMapper.h"

//#include <comdef.h>

#include "Common/MemoryLeaks.h"


DX11Initializer::DX11Initializer()
{
	m_rasterizer = nullptr;
	m_depthState = nullptr;
}

/**@brief Tworzy renderer zgodny z u¿ywanym API graficznym.

@param[in] usage Specyfikuje czy u¿yæ opóŸnionego kontekstu renderowania czy natychmiastowego.
@note Wszystkie renderery stworzone w aplikacji powinny byæ tego samego typu.*/
IRenderer* DX11Initializer::CreateRenderer( RendererUsage usage )
{
	return new DX11Renderer( usage );
}

/**@brief Tworzy nowy swap chain.*/
SwapChain* DX11Initializer::CreateSwapChain( SwapChainInitData& swapChainData )
{
	assert( !"Implement me" );
	return nullptr;
}

/**@brief Inicjalizuje API graficzne.

@param[in] initData Dane u¿ywane do inicjalizacji.
@return Zwraca true, je¿eli inicjalizacja siê uda.*/
bool DX11Initializer::InitAPI( GraphicAPIInitData& initData )
{
	set_depth_stencil_format( DX11ConstantsMapper::Get( initData.SwapChain.DepthStencilFormat ) );

	DX11_INIT_RESULT result;
	if( initData.CreateSwapChain )
	{
		result = init_DX11( initData.SwapChain.WindowWidth,
							initData.SwapChain.WindowHeight,
							(HWND)initData.SwapChain.WindowHandle,
							initData.SwapChain.FullScreen,
							initData.SingleThreaded );

		if( result != DX11_INIT_RESULT::DX11_INIT_OK )
			return false;
	}
	else
	{
		auto result = InitDevices( initData );
		if( !result.IsValid )
			return false;
	}

	result = init_sampler();
	if ( result != DX11_INIT_OK )
	{
		release_DirectX();	// Jak tu coœ siê nie uda³o, to znaczy, ¿e deskryptor by³ niepoprawny.
		return false;
	}

	if( FAILED( device->CreateRasterizerState( &get_rasterizer_desc(), &m_rasterizer ) ) )
		return false;

	device_context->RSSetState( m_rasterizer );

	//D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	//depthStencilDesc.DepthEnable = true;
	//depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	//depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	//depthStencilDesc.StencilEnable = false;
	//depthStencilDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	//depthStencilDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
	//depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	//depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	//depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	//depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	//depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	//depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	//depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	//depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	//if( FAILED( device->CreateDepthStencilState( &depthStencilDesc, &m_depthState ) ) )
	//	return false;

	//device_context->OMSetDepthStencilState( m_depthState, 0 );

	return true;
}

/**@brief Zwalnia stworzone obiekty DirectX 11.*/
void DX11Initializer::ReleaseAPI()
{
	if( m_rasterizer )
		m_rasterizer->Release();
	m_rasterizer = nullptr;

	if( m_depthState )
		m_depthState->Release();
	m_depthState = nullptr;

	release_DirectX();
}

/**@brief Zwraca wskaŸnik na bufor koloru render targetu.

Ta funkcja jest u¿ywana do wspó³pracy z zewnêtrznymi aplikacjami, które potrzebuj¹ bezpoœredniego
wskaŸnika na zasoby DirectXowe. Nie ma ¿adnego powodu, ¿eby jej u¿ywaæ w innych przypadkach.

@param[in] renderTarget RenderTarget, z którego ma zostaæ wziêtu wskaŸnik.
@return Zwraca wskaŸnik na widok na bufor koloru render targetu lub nullptr,
jezeli podano niepoprawny render target.*/
void* DX11Initializer::GetRenderTargetHandle( RenderTargetObject* renderTarget )
{
	DX11RenderTarget* renderTargetDX11 = static_cast<DX11RenderTarget*>( renderTarget );

	if( renderTargetDX11 )
	{
		auto colorBufferTex = static_cast<DX11Texture*>( renderTargetDX11->GetColorBuffer() );
		if( colorBufferTex )
		{
			ID3D11Resource* renderTargetTexture;
			auto colorBufferView = colorBufferTex->Get();
			colorBufferView->GetResource( &renderTargetTexture );
			// Uwaga! GetResource zwiêksza liczbê referencji do tekstury. Dlatego tutaj j¹ zmniejszamy.
			// Tekstura i tak nie zostanie zwolniona, bo ResourceView trzymaj¹ na ni¹ referencjê.
			renderTargetTexture->Release();
			return renderTargetTexture;
		}
	}
	return nullptr;
}

/**@brief Creates only device and device context.*/
Nullable< bool >	DX11Initializer::InitDevices	( GraphicAPIInitData& initData )
{
	UINT createDeviceFlags = 0;


	if( initData.UseDebugLayer )
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;

	if ( initData.SingleThreaded )
		// Domyœlnie obiekt ID3D11Device jest synchronizowany, ale mo¿na to wy³¹czyæ
		createDeviceFlags |= D3D11_CREATE_DEVICE_SINGLETHREADED;

	auto result = D3D11CreateDevice( nullptr, D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, _feature_levels, _num_feature_levels, D3D11_SDK_VERSION, &device, &_current_feature_level, &device_context );
	if ( FAILED( result ) )
		return "Creating device failed.";	//@todo Use _com_error( result ).ErrorMessage();


	if( initData.UseDebugLayer )
	{
		result = device->QueryInterface( __uuidof( ID3D11Debug ), (void**)&debug_interface );
		if ( FAILED( result ) )
			return "Creating debug layer failed";
	}

	return true;
}
