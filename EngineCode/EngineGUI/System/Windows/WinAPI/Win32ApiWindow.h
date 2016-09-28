#pragma once

#include "EngineGUI/System/Windows/INativeWindow.h"


#include <Windows.h>
#undef CreateWindow

namespace GUI
{


/**@brief Native window uses WinApi.

@todo Move to separate library in future.*/
class Win32ApiWindow :	public INativeWindow
{
private:

	HWND			m_windowHandle;
	std::wstring	m_title;

	uint16			m_width;
	uint16			m_height;

	ResourcePtr< RenderTargetObject >	m_renderTarget;
	ResourcePtr< SwapChain >			m_swapChain;

public:
	explicit			Win32ApiWindow	();
						~Win32ApiWindow	();


	static Win32ApiWindow*		CreateWindowInstance	( NativeWindowDescriptor& descriptor );

	// Inherited via INativeWindow
	virtual ResourcePtr< RenderTargetObject >		GetRenderTarget		() override;
	virtual ResourcePtr< SwapChain >				GetSwapChain		() override;

private:
	void			ShowAppWindow			();
	void			HideAppWindow			();

	bool			Initialize				( NativeWindowDescriptor& descriptor );

};


}	// GUI
