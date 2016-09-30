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

public:
	explicit			Win32ApiWindow	();
						~Win32ApiWindow	();


	static Win32ApiWindow*		CreateWindowInstance	( NativeWindowDescriptor& descriptor );

private:
	void			ShowAppWindow			();
	void			HideAppWindow			();

	bool			Initialize				( NativeWindowDescriptor& descriptor );


	// Inherited via INativeWindow
	virtual uint16				GetWidth		() override;
	virtual uint16				GetHeght		() override;

	virtual WindowHandler		GetHandle		() override;

	virtual std::string			GetTitle		() override;

	virtual void				Show			() override;
	virtual void				Hide			() override;

	virtual void				SetTitle		( const std::string & newTitle ) override;

};


}	// GUI
