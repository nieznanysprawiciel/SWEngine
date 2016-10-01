#include "WinAPIGUI.h"
#include "Win32ApiWindow.h"

#include "InputLibrary/InputWinApi/WinApiInputProxy.h"
#include <Windows.h>



namespace GUI
{


LRESULT CALLBACK		WindowProcedure		( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
INativeWindow*			GetNativePointer	( HWND window );


WinAPIGUI::WinAPIGUI()
	:	m_input( nullptr )
{}

/**@brief Creates WinAPIGUI object.*/
WinAPIGUI*		WinAPIGUI::Create()
{
	auto nativeGUI = new WinAPIGUI();
	bool result = nativeGUI->Init();
	assert( result );

	return nativeGUI;
}



//====================================================================================//
//			Window creation	
//====================================================================================//


const wchar_t WINDOW_CLASS_NAME[] = L"GUI Window";

/**@brief Window class registered in WinAPI.*/
const wchar_t*	WinAPIGUI::GetWindowClassName()
{
	return WINDOW_CLASS_NAME;
}

IInput*			WinAPIGUI::UseNativeInput()
{
	InputInitInfo init;
	init.AppInstance = GetModuleHandle( nullptr );
	init.WndHandle = nullptr;

	m_input = new WinApiInputProxy();
	if( !m_input->Init( init ) )
	{
		assert( !"Initialziation failed" );
		return nullptr;
	}

	return m_input;
}

// WinAPI macros :(
#undef CreateWindow

/**@copydoc INativeGUI::CreateWindow*/
INativeWindow*	WinAPIGUI::CreateWindow		( NativeWindowDescriptor& descriptor )
{
	return Win32ApiWindow::CreateWindowInstance( descriptor );
}


//====================================================================================//
//			Initialization	
//====================================================================================//


/**@brief Registers window class.*/
void		WinAPIGUI::RegisterWindowClass()
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof( WNDCLASSEX );

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WindowProcedure;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = sizeof( Win32ApiWindow );
	wcex.hInstance = GetModuleHandle( nullptr );
	wcex.hIcon = LoadIcon( wcex.hInstance, IDI_APPLICATION );
	wcex.hCursor = LoadCursor( NULL, IDC_ARROW );
	wcex.hbrBackground = (HBRUSH)( COLOR_WINDOW + 1 );
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = WINDOW_CLASS_NAME;
	wcex.hIconSm = LoadIcon( wcex.hInstance, IDI_APPLICATION );

	auto result = RegisterClassEx( &wcex );
	if( result == 0 )
		PrintLastError();
}


/**@brief Gets last win api error and prints to debug window.*/
void		WinAPIGUI::PrintLastError()
{
	LPVOID messageBuffer;
	DWORD error = GetLastError();
	FormatMessage(	FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
					NULL, error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
					(LPTSTR)&messageBuffer, 0, NULL );
	OutputDebugString( (LPCTSTR)messageBuffer );
	LocalFree( messageBuffer );
}

/**@brief Retrives INativeWindow pointer hidden in native window extra bytes.*/
INativeWindow*		GetNativePointer( HWND window )
{
	return (INativeWindow*)GetWindowLongPtr( window, 0 );
}


/**@copydoc INativeGUI::Init*/
bool		WinAPIGUI::Init()
{
	RegisterWindowClass();

	return true;
}



//====================================================================================//
//			Window Message handler
//====================================================================================//

/**@brief Core functionality of main loop function.*/
bool		WinAPIGUI::MainLoopCore( MSG* msg )
{
	TranslateMessage( msg );

	if( m_input )
		m_input->HandleEvent( msg->hwnd, msg->message, msg->wParam, msg->lParam );

	DispatchMessage( msg );

	if( msg->message == WM_QUIT )
	{
		return true;
	}
	return false;
}




/**@copydoc INativeGUI::MainLoop*/
bool		WinAPIGUI::MainLoop( bool blockingMode )
{
	MSG msg;

	if( blockingMode )
	{
		GetMessage( &msg, NULL, 0, 0 );
		do
		{
			bool end = MainLoopCore( &msg );
			if( end ) return true;

		} while( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) );
	}
	else
	{
		while( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
		{
			bool end = MainLoopCore( &msg );
			if( end ) return true;
		}
	}

	return false;
}


/**@brief Window procedure.*/
LRESULT CALLBACK		WindowProcedure		( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch( message )
	{
	case WM_PAINT:
		hdc = BeginPaint( hWnd, &ps );
		// TODO: Add any drawing code here...
		EndPaint( hWnd, &ps );
	break;
	case WM_DESTROY:
		PostQuitMessage( 0 );
		break;
	default:
		return DefWindowProc( hWnd, message, wParam, lParam );
	}
	return 0;
}


}	// GUI
