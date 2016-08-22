#pragma once

#include "GraphicAPI/DX11API/DX11Initializer/DX11APIObjects.h"
#include "GraphicAPI/SwapChain.h"
#include "DX11RenderTarget.h"


class DX11SwapChain	: public SwapChain, public DX11APIObjects
{
	RTTR_ENABLE( SwapChain )
private:

	IDXGISwapChain*			m_swapChain;

public:
	DX11SwapChain( IDXGISwapChain* chain, DX11RenderTarget* windowRT );
	~DX11SwapChain();

	virtual void			Present			( int syncInterval )					override;
	virtual void			Resize			( uint16 newWidth, uint16 newHeight )	override;

public:
	static DX11SwapChain*	CreateScreenSwapChain	( RenderTargetObject* screenRT );
};

