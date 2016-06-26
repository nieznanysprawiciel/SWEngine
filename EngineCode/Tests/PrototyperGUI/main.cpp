#include "EngineGUI/System/GUISystem.h"

#include "Application.h"


int main( int argc, char** argv )
{
	Application mainClass;
	
	GUI::GUISystem app( argc, argv );
	app.DataContext() = &mainClass;

	return app.MainLoop();
}