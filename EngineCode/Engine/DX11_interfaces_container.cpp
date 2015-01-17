#include "..\stdafx.h"
#include "DX11_interfaces_container.h"



/*Tworzy obiekty swap_chain, device i device_context.*/

int DX11_interfaces_container::init_devices( int width, int height, HWND window, bool fullscreen, bool single_thread )
{
	HRESULT result = S_OK;
	UINT createDeviceFlags = 0;

#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	if ( single_thread )
		//Renderujemy z jednego w¹tku, a o synchronizacjê dbamy sami
		createDeviceFlags |= D3D11_CREATE_DEVICE_SINGLETHREADED;

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	UINT numFeatureLevels = ARRAYSIZE( featureLevels );


	DXGI_SWAP_CHAIN_DESC swap_chain_desc;
	ZeroMemory( &swap_chain_desc, sizeof(swap_chain_desc) );
	swap_chain_desc.BufferCount = 1;
	swap_chain_desc.BufferDesc.Width = width;
	swap_chain_desc.BufferDesc.Height = height;
	swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swap_chain_desc.BufferDesc.RefreshRate.Numerator = 60;
	swap_chain_desc.BufferDesc.RefreshRate.Denominator = 1;
	swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swap_chain_desc.OutputWindow = window;
	swap_chain_desc.SampleDesc.Count = 1;
	swap_chain_desc.SampleDesc.Quality = 0;
	swap_chain_desc.Windowed = TRUE;
	//swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
	swap_chain_desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	result = D3D11CreateDeviceAndSwapChain( nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
											createDeviceFlags, featureLevels, numFeatureLevels,
											D3D11_SDK_VERSION, &swap_chain_desc, &swap_chain,
											&device, &feature_level, &device_context );
	if ( result != GRAPHIC_ENGINE_INIT_OK )
		return result;

	if ( fullscreen )
		swap_chain->SetFullscreenState( TRUE, nullptr );


	// Tworzymy RenderTargetView. W tym celu pobieramy wskaŸnik na obiekt tylniego bufora
	// i tworzymy z niego widok.
	ID3D11Texture2D* back_buffer = nullptr;
	result = swap_chain->GetBuffer( 0, __uuidof(ID3D11Texture2D), (LPVOID*)&back_buffer );
	if ( result != GRAPHIC_ENGINE_INIT_OK )
	{
		release_DirectX( );
		return result;
	}

	result = device->CreateRenderTargetView( back_buffer, nullptr, &render_target );
	if ( result != GRAPHIC_ENGINE_INIT_OK )
	{
		release_DirectX( );
		return result;
	}
	//Zwracam uwagê, ¿e tutaj nie zwalniamy bufora. Zwalniamy tylko odwo³anie do niego, a bufor
	//zostanie zwolniony dopiero jak wszystkie istniej¹ce obiekty przestan¹ go u¿ywaæ.
	back_buffer->Release( );

	// Deklarujemy przekszta³cenie wspó³rzêdnych ekranowych, na wspó³rzedne znormalizowane
	D3D11_VIEWPORT view_port;
	view_port.Width = (FLOAT)width;
	view_port.Height = (FLOAT)height;
	view_port.MinDepth = 0.0f;
	view_port.MaxDepth = 1.0f;
	view_port.TopLeftX = 0;
	view_port.TopLeftY = 0;
	device_context->RSSetViewports( 1, &view_port );	//tworzymy tylko jeden viewport, ale noramlnie mo¿na wiecej, tylko po co

	return GRAPHIC_ENGINE_INIT_OK;
}

/*Funkcja tworzy z-bufffer. Nastêpnie widok z bufora i widok tylnego bufora
s¹ ustawione jako cel dla funkcji renderuj¹cych renderowania.

Funkcji nie wolno wywo³ywaæ przed zainicjowaniem directXa.*/
int DX11_interfaces_container::init_zBuffer( int width, int height )
{
	if ( !device )
		return DXGI_ERROR_ACCESS_DENIED;

	HRESULT result = S_OK;
	ID3D11Texture2D* zBuffer;


	D3D11_TEXTURE2D_DESC zBuffer_desc;
	ZeroMemory( &zBuffer_desc, sizeof(zBuffer_desc) );
	zBuffer_desc.Width = width;
	zBuffer_desc.Height = height;
	zBuffer_desc.MipLevels = 1;
	zBuffer_desc.ArraySize = 1;
	zBuffer_desc.Format = DXGI_FORMAT_D32_FLOAT;
	zBuffer_desc.SampleDesc.Count = 1;
	zBuffer_desc.SampleDesc.Quality = 0;
	zBuffer_desc.Usage = D3D11_USAGE_DEFAULT;
	zBuffer_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	zBuffer_desc.CPUAccessFlags = 0;
	zBuffer_desc.MiscFlags = 0;

	result = device->CreateTexture2D( &zBuffer_desc, nullptr, &zBuffer );

	if ( result != GRAPHIC_ENGINE_INIT_OK )
	{
		release_DirectX( );
		return result;
	}

	// Create the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC zBufferView_desc;
	ZeroMemory( &zBufferView_desc, sizeof(zBufferView_desc) );
	zBufferView_desc.Format = zBuffer_desc.Format;
	zBufferView_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	zBufferView_desc.Texture2D.MipSlice = 0;

	result = device->CreateDepthStencilView( zBuffer, &zBufferView_desc, &z_buffer_view );

	zBuffer->Release();

	if ( result != GRAPHIC_ENGINE_INIT_OK )
	{
		release_DirectX( );
		return result;
	}

	device_context->OMSetRenderTargets( 1, &render_target, z_buffer_view );

	return GRAPHIC_ENGINE_INIT_OK;
}



void DX11_interfaces_container::release_DirectX( )
{
	if ( swap_chain )
		//DirectX nie potrafi siê zamkn¹æ w trybie pe³noekranowym, wiêc musimy go zmieniæ
		swap_chain->SetFullscreenState( FALSE, NULL );
	if ( z_buffer_view )
		z_buffer_view->Release( );
	if ( swap_chain )
		swap_chain->Release();
	if ( render_target )
		render_target->Release();
	if ( device )
		device->Release();
	if ( device_context )
		device_context->Release();
}


/*Czyœcimy tylny bufor oraz z-bufor i wywo³ujemy funcjê inicjuj¹c¹ renderowanie.*/
void DX11_interfaces_container::begin_scene()
{
	//Bufor tylny
	float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };	// red, green, blue, alpha
	device_context->ClearRenderTargetView( render_target, ClearColor );

	//Z-bufor
	device_context->ClearDepthStencilView( z_buffer_view, D3D11_CLEAR_DEPTH, 1.0f, 0 );
}

