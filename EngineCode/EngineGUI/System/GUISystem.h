#pragma once

#include "Common/EngineObject.h"
#include "HostWindow.h"


namespace GUI
{



/**@brief G³ówna klasa obs³uguj¹ca gui aplikacji.

@ingroup EngineGUI

Przyk³ad u¿ycia GUI:
@code

int main()
{
	YourMainApplicationClass mainClass;
	
	GUI::GUISystem app;
	app.DataContext() = &mainClass;

	return app.MainLoop();
}
@endcode
*/
class GUISystem
{
private:
protected:

	EngineObject*				m_dataContext;

	std::vector< HostWindow* >	m_windows;

public:
	explicit		GUISystem		() = default;
					~GUISystem	() = default;

	int				MainLoop		();


	EngineObject*&	DataContext		();

	Size			GetMemorySize	();
};


}	// GUI
