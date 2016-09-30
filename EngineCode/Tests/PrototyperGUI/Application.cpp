#include "Application.h"



// ================================ //
//
Application::Application	( int argc, char** argv, GUI::INativeGUI* gui )
	:	GUI::GUISystem( argc, argv, gui )
{}


/**@brief GUI subsystems initialization.

If you need specific gui initialization in your application override this function.
You can set different GraphicApi or input api.*/
void Application::Initialize()
{
	DefaultInit( 1024, 768, "Window Tittle" );
}

/**@brief Function is called when GUI initialization is completed.

In this function you should initialize your application logic.
*/
void		Application::OnInitialized()
{ }

/**@brief Function invoked when application is going to close itself.*/
void		Application::OnClosing()
{ }

/**@brief */
void		Application::OnIdle()
{ }
