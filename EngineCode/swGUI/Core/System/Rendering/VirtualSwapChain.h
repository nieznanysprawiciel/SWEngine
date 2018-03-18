#pragma once

#include "swGraphicAPI/Resources/SwapChain.h"


namespace sw {
namespace gui
{


/**@brief SwapChain for virtual window.

Virtual windows are simple rendering surfaces without underlying native window,
but they can contain controls and everything like normal windows. We need VirtualSwapChain
to hold RenderTarget and replace grpahic API swap chain.*/
class VirtualSwapChain : public SwapChain
{
	RTTR_ENABLE( SwapChain )
private:
public:

	explicit				VirtualSwapChain	( RenderTargetObject* screenRT );
	virtual					~VirtualSwapChain	();

	virtual void			Present			( int syncInterval )					override;
	virtual void			Resize			( uint16 newWidth, uint16 newHeight )	override;

public:
	static VirtualSwapChain*	CreateVirtualSwapChain	( RenderTargetObject* screenRT );
};

}	// gui
}	// sw

