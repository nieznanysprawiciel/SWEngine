/**
@file DX11Utils.cpp
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/
#include "swGraphicAPI/DX11API/stdafx.h"

#include "DX11Utils.h"

#include "DX11ConstantsMapper.h"



// ================================ //
//
DXGI_SWAP_CHAIN_DESC			DX11Utils::CreateSwapChainDesc		( const SwapChainInitData& swapChainData )
{
	DXGI_SWAP_CHAIN_DESC desc;
	desc.BufferCount = swapChainData.NumBuffers;
	desc.BufferDesc.Format = DX11ConstantsMapper::Get( swapChainData.BackBufferFormat );
	desc.BufferDesc.Height = swapChainData.WindowHeight;
	desc.BufferDesc.Width = swapChainData.WindowWidth;
	desc.OutputWindow = (HWND)swapChainData.WindowHandle;
	desc.SampleDesc.Count = swapChainData.SamplesCount;
	desc.SampleDesc.Quality = swapChainData.SamplesQuality;

	return desc;
}

// ================================ //
//
ComPtr< IDXGIAdapter >			DX11Utils::GetDXGIAdapter			()
{
	HRESULT result;

	ComPtr< IDXGIDevice > dxgiDevice = nullptr;
	result = device->QueryInterface( __uuidof( IDXGIDevice ), (void**)&dxgiDevice );
	
	assert( SUCCEEDED( result ) );
	if( FAILED( result ) )	return nullptr;

	ComPtr< IDXGIAdapter > dxgiAdapter = nullptr;
	result = dxgiDevice->GetParent( __uuidof( IDXGIAdapter ), (void **)&dxgiAdapter );
	
	assert( SUCCEEDED( result ) );
	if( FAILED( result ) )	return nullptr;

	return dxgiAdapter;
}

// ================================ //
//
ComPtr< IDXGIFactory >			DX11Utils::GetDXGIFactory			()
{
	HRESULT result;

	ComPtr< IDXGIAdapter > dxgiAdapter = DX11Utils::GetDXGIAdapter();
	if( dxgiAdapter )
	{
		ComPtr< IDXGIFactory > dxgiFactory = nullptr;
		result = dxgiAdapter->GetParent( __uuidof( IDXGIFactory ), (void **)&dxgiFactory );

		assert( SUCCEEDED( result ) );
		if( FAILED( result ) )	return nullptr;

		return dxgiFactory;
	}

	return nullptr;
}

