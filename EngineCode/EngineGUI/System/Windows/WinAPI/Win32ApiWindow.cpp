#include "Win32ApiWindow.h"

#include "WinAPIGUI.h"
#include "Common/Converters.h"


namespace GUI
{




Win32ApiWindow::Win32ApiWindow()
{}


Win32ApiWindow::~Win32ApiWindow()
{}

/**@brief */
Win32ApiWindow*		Win32ApiWindow::CreateWindowInstance	( NativeWindowDescriptor& descriptor )
{
	Win32ApiWindow* nativeWindow = new Win32ApiWindow();
	if( !nativeWindow->Initialize( descriptor ) )
	{
		delete nativeWindow;
		return nullptr;
	}

	return nativeWindow;
}

/**@brief */
bool		Win32ApiWindow::Initialize	( NativeWindowDescriptor& descriptor )
{
	HINSTANCE instance = GetModuleHandle( nullptr );

	m_title = Convert::FromString< std::wstring >( descriptor.WindowTitle, L"Remember about title" );


	int windowStyle = 0;
	if( descriptor.AddTitleBar )
		windowStyle = windowStyle | WS_CAPTION;
	if( descriptor.AddFrame )
		windowStyle = windowStyle | WS_THICKFRAME;
	if( descriptor.AddMinimizeButton )
		windowStyle = windowStyle | WS_MINIMIZEBOX | WS_SYSMENU;
	if( descriptor.AddMaximizeButton )
		windowStyle = windowStyle | WS_MAXIMIZEBOX | WS_SYSMENU;
	if( descriptor.Fullscreen )
		windowStyle = windowStyle | WS_EX_TOPMOST;

	if( !descriptor.AddTitleBar &&
		!descriptor.AddFrame &&
		!descriptor.AddMinimizeButton &&
		!descriptor.AddMaximizeButton )
	{
		windowStyle = WS_POPUP;
	}


	if( !descriptor.Fullscreen && descriptor.AdjustSize )
	{
		RECT windowRect ={ 0, 0, descriptor.Width, descriptor.Height };
		AdjustWindowRect( &windowRect, windowStyle, FALSE );

		descriptor.Width = static_cast< uint16 >( windowRect.right - windowRect.left );
		descriptor.Height = static_cast< uint16 >( windowRect.bottom - windowRect.top );
	}

	m_width = descriptor.Width;
	m_height = descriptor.Height;


	if( descriptor.Fullscreen )
	{
		m_windowHandle = CreateWindowEx( NULL, WinAPIGUI::GetWindowClassName(), m_title.c_str(), windowStyle, //WS_EX_TOPMOST | WS_POPUP,
												  0, 0, m_width, m_height,
												  (HWND)descriptor.ParentHandle, NULL, instance, NULL );
	}
	else
	{
		m_windowHandle = CreateWindowEx( NULL, WinAPIGUI::GetWindowClassName(), m_title.c_str(), windowStyle, //WS_OVERLAPPEDWINDOW^WS_THICKFRAME,
												  descriptor.PositionX, descriptor.PositionY, m_width, m_height,
												  (HWND)descriptor.ParentHandle, NULL, instance, NULL );
	}

	if( !m_windowHandle )
	{
		WinAPIGUI::PrintLastError();
		return false;
	}

	if( descriptor.ShowWindow )
		ShowAppWindow();

	return true;
}

// ================================ //
//
ResourcePtr<RenderTargetObject> Win32ApiWindow::GetRenderTarget()
{
	return ResourcePtr<RenderTargetObject>();
}

// ================================ //
//
ResourcePtr<SwapChain> Win32ApiWindow::GetSwapChain()
{
	return ResourcePtr<SwapChain>();
}




/**@brief */
void		Win32ApiWindow::ShowAppWindow()
{
	ShowWindow( m_windowHandle, SW_SHOWNORMAL );
	UpdateWindow( m_windowHandle );
}

/**@brief */
void		Win32ApiWindow::HideAppWindow()
{
	ShowWindow( m_windowHandle, SW_HIDE );
}




}	// GUI

