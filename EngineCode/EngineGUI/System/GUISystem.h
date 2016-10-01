#pragma once

#include "Common/EngineObject.h"
#include "GraphicAPI/IGraphicAPIInitializer.h"
#include "EngineGUI/System/Windows/INativeGUI.h"

#include "HostWindow.h"

class ResourceManager;


namespace GUI
{


struct CommandLineArgs
{
	int				ArgsCount;
	char**			Arguments;


	CommandLineArgs( int argc, char** argv )
		:	ArgsCount( argc )
		,	Arguments( argv )
	{}

	const char*		ProgramName	()	{ return Arguments[ 0 ];}
};


/**@brief Main GUI application class.

@ingroup EngineGUI

Use example of GUI:
@code
#include "EngineGUI/System/GUISystem.h"
#include "EngineGUI/System/Windows/WinAPI/WinAPIGUI.h"

#include "Application.h"

int main( int argc, char** argv )
{
	Application app( argc, argv, GUI::WinAPIGUI::Create() );
	app.Init();

	return app.MainLoop();
}
@endcode

You don't have to write main function. Instead use project templates for different platforms.
To use gui implement Application class.

@todo Supply project templates.
*/
class GUISystem
{
private:

	static GUISystem*			m_instance;

protected:

	IGraphicAPIInitializer*		m_graphicApi;		///< Contains object responsible for dealing with specifics graphic apis.
	INativeGUI*					m_nativeGUI;		///< Native window system used to display main application window.
	IInput*						m_input;			///< Input processor.
	ResourceManager*			m_resourceManager;	///< Resources.

	std::vector< HostWindow* >	m_windows;
	HostWindow*					m_focusedWindow;	///< Only one window can have focus and only this will be processed.

	CommandLineArgs				m_cmdArgs;

public:
	explicit		GUISystem		( int argc, char** argv, INativeGUI* gui );
	virtual			~GUISystem		();

	int				MainLoop		();


	Size			GetMemorySize	();

	int				NumCommandLineArgs	();
	const char*		CommandLineArg		( int num );
	const char*		ProgramPath			();

protected:
	virtual	void	Initialize		();
	virtual void	OnInitialized	() = 0;
	virtual void	OnClosing		() = 0;
	virtual void	OnIdle			() = 0;

	void			DefaultInit		( uint16 width, uint16 height, const std::string& windowTitle );

public:
	void			Init			();

public:
	static GUISystem&	Get			();
};


}	// GUI
