#pragma once


#include "INativeWindow.h"

namespace GUI
{



/**@brief Encasulates native GUI system.*/
class INativeGUI
{
private:
protected:
public:
	INativeGUI() = default;
	~INativeGUI() = default;


	/**@brief Invokes main loop of native GUI.
	@return Returns true if application should close.*/
	virtual bool			MainLoop		( bool blockingMode ) = 0;

	/**@brief Initializes native GUI.
	@return Returns false if initialization failed.*/
	virtual bool			Init			() = 0;

	/**@brief Creates native GUI window.*/
	virtual INativeWindow*	CreateWindow	( NativeWindowDescriptor& descriptor ) = 0;
};


}	// GUI

