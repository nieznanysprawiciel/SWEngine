#pragma once


#include "EngineGUI/System/Windows/INativeGUI.h"

namespace GUI
{


/**@brief */
class WinAPIGUI : public INativeGUI
{
private:
protected:
public:
	WinAPIGUI() = default;
	~WinAPIGUI() = default;


	// Inherited via INativeGUI
	virtual bool				MainLoop		( bool blockingMode ) override;
	virtual bool				Init			() override;
	virtual IInput*				UseNativeInput	() override;
	virtual INativeWindow*		CreateWindow	( NativeWindowDescriptor & descriptor ) override;

	static WinAPIGUI*			Create			();

private:
	void					RegisterWindowClass		();

public:
	// Helpers
	static void				PrintLastError			();
	static const wchar_t*	GetWindowClassName		();

};

}	// GUI
