#pragma once

#include "EngineGUI/System/Windows/INativeWindow.h"
#include "EngineGUI/Controls/IControl.h"
#include "EngineGUI/Controls/TopLevelControl.h"

#include "InputLibrary/IInput.h"

#include <vector>
#include <map>

class IGraphicAPIInitializer;

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
	IInput*						m_input;
	ResourceManager*			m_resourceManager;

	EngineObject*				m_dataContext;

	std::vector< IControl* >	m_mousePath;		///< Controls hierarchy that captured mouse in this frame.
	std::vector< IControl* >	m_focusPath;		///< Controls hierarchy that have focus in this frame.
	std::vector< IControl* >	m_invalidated;		///< Controls which needs to be redrawn in this frame.

	std::vector< TopLevelControl* >		m_controlTree;	///< Top level controls.

	ResourcePtr< RenderTargetObject >	m_renderTarget;
	ResourcePtr< SwapChain >			m_swapChain;

	///@name Controls info
	///@{

	/// Map containing windows names. Most controls don't have name, so it's better to store
	/// them separatly, to lower memory consumption.
	std::map< IControl*, std::string >	m_controlsNames;

	///@}

protected:
public:
	explicit		HostWindow	( INativeWindow* nativeWindow, IInput* input, ResourceManager* resourceManager, IGraphicAPIInitializer* graphicApi );
					~HostWindow	();


	Size				GetMemorySize		();


	EngineObject*&		DataContext			();
	void				RemoveControl		( IControl* control );

	void				RegisterControlName	( IControl* control, const std::string& name );
	const std::string&	GetControlName		( IControl* control );


	ResourcePtr< RenderTargetObject >	GetRenderTarget		();
	ResourcePtr< SwapChain >			GetSwapChain		();
};


}	// GUI
