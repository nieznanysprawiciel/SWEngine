#pragma once


#include "EngineGUI/System/Windows/INativeGUI.h"


class WinApiInputProxy;
struct tagMSG;
typedef tagMSG MSG;



namespace GUI
{


/**@brief */
class WinAPIGUI : public INativeGUI
{
private:
	WinApiInputProxy*		m_input;

protected:
public:
	explicit			WinAPIGUI();
						~WinAPIGUI() = default;


	// Inherited via INativeGUI
	virtual bool				MainLoop		( bool blockingMode ) override;
	virtual bool				Init			() override;
	virtual IInput*				UseNativeInput	() override;
	virtual INativeWindow*		CreateWindow	( NativeWindowDescriptor & descriptor ) override;

	static WinAPIGUI*			Create			();

private:
	void					RegisterWindowClass		();
	bool					MainLoopCore			( MSG* msg );

public:
	// Helpers
	static void				PrintLastError			();
	static const wchar_t*	GetWindowClassName		();

};

}	// GUI
