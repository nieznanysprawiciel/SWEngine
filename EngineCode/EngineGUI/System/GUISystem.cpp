#include "GUISystem.h"

namespace GUI
{



/**@brief G³ówna pêtla aplikacji.

@see Application
*/
int					GUISystem::MainLoop()
{
	return 0;
}

/**@brief Pozwala ustawiæ i pobraæ DataContext.

DataContext zostanie przepropagowany do wszystkich kontrolek w hierarchii.*/
EngineObject*&		GUISystem::DataContext()
{
	return m_dataContext;
}

}	// GUI

