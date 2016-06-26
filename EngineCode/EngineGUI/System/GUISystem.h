#pragma once

#include "Common/EngineObject.h"
#include "HostWindow.h"


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

int main()
{
	YourMainApplicationClass mainClass;
	
	GUI::GUISystem app( argc, argv );
	app.DataContext() = &mainClass;

	return app.MainLoop();
}
@endcode
*/
class GUISystem
{
private:

	static GUISystem*			m_instance;

protected:

	EngineObject*				m_dataContext;

	std::vector< HostWindow* >	m_windows;

	CommandLineArgs				m_cmdArgs;

public:
	explicit		GUISystem		( int argc, char** argv );
					~GUISystem		() = default;

	int				MainLoop		();


	EngineObject*&	DataContext		();

	Size			GetMemorySize	();

	int				NumCommandLineArgs	();
	const char*		CommandLineArg		( int num );
	const char*		ProgramPath			();

public:
	static GUISystem&	Get			();
};


}	// GUI
