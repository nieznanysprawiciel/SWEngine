#pragma once

#include "DX11APIObjects.h"
#include "swGraphicAPI/Rendering/IGraphicAPIInitializer.h"



/**@brief Helper functions for DirecX objects managment.*/
class DX11Utils : protected DX11APIObjects
{
private:
protected:
public:
	explicit		DX11Utils		() = default;
	~DX11Utils	() = default;


public:

	static DXGI_SWAP_CHAIN_DESC			CreateSwapChainDesc		( const SwapChainInitData& swapChainData);


public:

	static ComPtr< IDXGIFactory >				GetDXGIFactory			();
	static ComPtr< IDXGIAdapter >				GetDXGIAdapter			();
};




