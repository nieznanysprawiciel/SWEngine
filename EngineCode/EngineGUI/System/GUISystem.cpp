#include "GUISystem.h"

#include "InputLibrary/KeyboardState.h"

#include <map>
#include <string>

namespace GUI
{

/// @todo This should be moved domewhere else.
INativeGUI*			CreateNativeGUI();



GUISystem*			GUISystem::m_instance = nullptr;


// ================================ //
//
GUISystem::GUISystem( int argc, char** argv )
	:	m_cmdArgs( argc, argv )
{
	m_instance = this;
	Init();
}

// ================================ //
//
GUISystem::~GUISystem()
{
	delete m_nativeGUI;
}


/**@brief */
void GUISystem::Init()
{
	m_nativeGUI = CreateNativeGUI();
	bool result = m_nativeGUI->Init();
	assert( result );

	NativeWindowDescriptor init( "My first window" );
	auto window = m_nativeGUI->CreateWindow( init );
	assert( window );

	HostWindow* hostWindow = new HostWindow( window, nullptr );
	m_windows.push_back( hostWindow );
}

/**@brief Application main loop.

@see Application*/
int					GUISystem::MainLoop()
{
	// Tests only
	int size = sizeof( IControl );
	int stringSize = sizeof( std::string );
	int mapSize = sizeof( std::map< std::string, void* > );
	int stateSize = sizeof( KeyState );

// ================================ //
//
	bool end = false;
	while( !end )
	{
		// Process native events.
		end = m_nativeGUI->MainLoop( true );

		// @todo Now display all objects.
	}

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

