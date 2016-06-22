#include "GUISystem.h"

#include "InputLibrary/KeyboardState.h"

#include <map>
#include <string>

namespace GUI
{



/**@brief G³ówna pêtla aplikacji.

@see Application
*/
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

}	// GUI

