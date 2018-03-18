#include "VirtualSwapChain.h"



namespace sw {
namespace gui
{


// ================================ //
//
VirtualSwapChain::VirtualSwapChain	( RenderTargetObject* rt )
	:	SwapChain( rt )
{}

// ================================ //
//
VirtualSwapChain::~VirtualSwapChain()
{}

// ================================ //
/// Do nothing.
/// @todo In future we could swap render targets. Consider.
void			VirtualSwapChain::Present		( int syncInterval )
{}

// ================================ //
/// @todo Implement.
void			VirtualSwapChain::Resize		( uint16 newWidth, uint16 newHeight )
{}

// ================================ //
//
VirtualSwapChain *			VirtualSwapChain::CreateVirtualSwapChain		( RenderTargetObject* rt )
{
	return new VirtualSwapChain( rt );
}

}	// gui
}	// sw


