#include "IControl.h"

#include "EngineGUI/Controls/TopLevelControl.h"
#include "EngineGUI/System/HostWindow.h"


RTTR_REGISTRATION
{
	rttr::registration::class_< GUI::IControl >( "GUI::IControl" );
	rttr::registration::class_< GUI::TopLevelControl >( "GUI::TopLevelControl" );
}


namespace GUI
{


/**@brief */
IControl::IControl( IControl* parent )
	:	m_parent( parent )
{ }

/**@brief */
IControl::~IControl()
{ }


/**@brief Zwraca najwy�sz� w hierarchi kontrolk�.*/
TopLevelControl*	IControl::GetRootControl()
{
	if( m_parent )
		return m_parent->GetRootControl();
	else
		return static_cast< TopLevelControl* >( this );
}

/**@brief Zwraca obiekt zarz�dzaj�cy GUI.*/
HostWindow*			IControl::GetHost()
{
	TopLevelControl* topLevel = GetRootControl();
	return topLevel->GetHostWindow();
}

/**@brief Gets name of control if it was set. Otherwise it returns empty string.*/
const std::string&	IControl::GetName()
{
	auto host = GetHost();
	return host->GetControlName( this );
}

}	// GUI
