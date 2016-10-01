#include "GUISystem.h"

#include "InputLibrary/KeyboardState.h"
#include "GraphicAPI/ResourcesFactory.h"
#include "GraphicAPI/ResourceManager.h"

#include <map>
#include <string>

namespace GUI
{

/// @todo This should be moved domewhere else.
INativeGUI*			CreateNativeGUI();



GUISystem*			GUISystem::m_instance = nullptr;


// ================================ //
//
GUISystem::GUISystem( int argc, char** argv, INativeGUI* gui )
	:	m_cmdArgs( argc, argv )
	,	m_nativeGUI( gui )
	,	m_focusedWindow( nullptr )
	,	m_resourceManager( nullptr )
	,	m_input( nullptr )
{
	m_instance = this;
}

// ================================ //
//
GUISystem::~GUISystem()
{
	for( auto window : m_windows )
		delete window;

	m_graphicApi->ReleaseAPI();
	delete m_resourceManager;
	delete m_graphicApi;
	delete m_nativeGUI;
}



/**@brief Invoke this function in application entry point (main).*/
void GUISystem::Init()
{
	Initialize();		// Initialize subsystems.
	OnInitialized();	// User initialization.
}

/**@brief GUI subsystems initialization.

If you need specific gui initialization in your application override this function.
You can set different GraphicApi or input api.*/
void				GUISystem::Initialize()
{
	DefaultInit( 1024, 768, "Window Tittle" );
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


/**@brief */
void				GUISystem::DefaultInit( uint16 width, uint16 height, const std::string& windowTitle )
{
	m_resourceManager = new ResourceManager();

	// ResourceFactory creates api which is linked as library.
	m_graphicApi = ResourcesFactory::CreateAPIInitializer();
	GraphicAPIInitData graphicApiData;
	graphicApiData.CreateSwapChain = false;		// We will create swap chain and render target later with window.

	auto result = m_graphicApi->InitAPI( graphicApiData );
	assert( result );

	NativeWindowDescriptor init( windowTitle );
	init.Height = height;
	init.Width = width;

	m_input = m_nativeGUI->UseNativeInput();
	assert( m_input );

	auto nativeWindow = m_nativeGUI->CreateWindow( init );
	assert( nativeWindow );

	HostWindow* hostWindow = new HostWindow( nativeWindow, m_input, m_resourceManager, m_graphicApi );
	m_focusedWindow = hostWindow;
	m_windows.push_back( hostWindow );
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

