#pragma once

#include "EngineGUI/System/Windows/INativeWindow.h"
#include "EngineGUI/Controls/IControl.h"
#include "EngineGUI/Controls/TopLevelControl.h"

#include <vector>
#include <map>


namespace GUI
{

/**@brief Root class for controls hierarchy, contains native window.

@ingroup EngineGUI

Children of this class must inherit from TopLevelControl. This means that top level classes must be
Windows, ContextMenus, PopupMenus or others similar.*/
class HostWindow
{
private:

	INativeWindow*				m_nativeWindow;
	//IInput*					m_input;

	std::vector< IControl* >	m_mousePath;		///< Controls hierarchy that captured mouse at this moment.
	std::vector< IControl* >	m_focusPath;		///< Controls hierarchy that have focus at this moment.
	std::vector< IControl* >	m_invalidated;		///< Controls which needs to be redrawn in this frame.

	std::vector< TopLevelControl* >		m_controlTree;	///< Top level controls.

	///@name Controls info
	///@{

	/// Map containing windows names. Most controls don't have name, so it's better to store
	/// them separatly, to lower memory consumption.
	std::map< IControl*, std::string >	m_controlsNames;

	///@}

protected:
public:
	explicit		HostWindow	() = default;
					~HostWindow	() = default;


	Size			GetMemorySize		();


	void				RemoveControl		( IControl* control );

	void				RegisterControlName	( IControl* control, const std::string& name );
	const std::string&	GetControlName		( IControl* control );
};


}	// GUI
