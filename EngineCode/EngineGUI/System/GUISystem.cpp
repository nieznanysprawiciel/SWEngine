#include "GUISystem.h"

#include "InputLibrary/KeyboardState.h"

#include <map>
#include <string>

namespace GUI
{

GUISystem*			GUISystem::m_instance = nullptr;


GUISystem::GUISystem( int argc, char** argv )
	:	m_cmdArgs( argc, argv )
{
	m_instance = this;

}


/**@brief Application main loop.

@see Application*/
int					GUISystem::MainLoop()
{
	int size = sizeof( IControl );
	int stringSize = sizeof( std::string );
	int mapSize = sizeof( std::map< std::string, void* > );
	int stateSize = sizeof( KeyState );

	return 0;
}

/**@brief Pozwala ustawiæ i pobraæ DataContext.

DataContext zostanie przepropagowany do wszystkich kontrolek w hierarchii.*/
EngineObject*&		GUISystem::DataContext()
{
	return m_dataContext;
}

/**@copydoc EngineObject::MemorySize*/
Size				GUISystem::GetMemorySize()
{
	Size size = sizeof( HostWindow );
	size += m_windows.capacity() * sizeof( HostWindow* );

	for( auto window : m_windows )
		size += window->GetMemorySize();

	return size;
}

/**@brief Gets number of commnad line arguments.

Function doesn't take program nameinto account .*/
int			GUISystem::NumCommandLineArgs()
{
	return m_cmdArgs.ArgsCount - 1;
}

/**@brief Returns command line argument. 0 is first argument
not program name.*/
const char* GUISystem::CommandLineArg( int num )
{
	return m_cmdArgs.Arguments[ num -1 ];
}

/**@brief Returns program name retrived from 0 command line argument.*/
const char* GUISystem::ProgramPath()
{
	return m_cmdArgs.ProgramName();
}

/**@brief */
GUISystem&	GUISystem::Get()
{
	return *m_instance;
}

}	// GUI

