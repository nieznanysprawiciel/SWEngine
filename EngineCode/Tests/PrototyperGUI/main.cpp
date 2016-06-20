#include "EngineGUI/System/GUISystem.h"

#include "Application.h"


int main()
{
	Application mainClass;
	
	GUI::GUISystem app;
	app.DataContext() = &mainClass;

	return app.MainLoop();
}