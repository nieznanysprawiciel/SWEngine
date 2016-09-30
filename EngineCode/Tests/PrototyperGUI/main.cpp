#include "EngineGUI/System/GUISystem.h"
#include "EngineGUI/System/Windows/WinAPI/WinAPIGUI.h"

#include "Application.h"


int main( int argc, char** argv )
{
	Application app( argc, argv, GUI::WinAPIGUI::Create() );
	app.Init();

	return app.MainLoop();
}