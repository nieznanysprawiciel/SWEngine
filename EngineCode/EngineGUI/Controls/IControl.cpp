#include "IControl.h"

#include "EngineGUI/Controls/TopLevelControl.h"


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


/**@brief Zwraca najwy¿sz¹ w hierarchi kontrolkê.*/
TopLevelControl*	IControl::GetRootControl()
{
	if( m_parent )
		return m_parent->GetRootControl();
	else
		return static_cast< TopLevelControl* >( this );
}

/**@brief Zwraca obiekt zarz¹dzaj¹cy GUI.*/
HostWindow*			IControl::GetHost()
{
	TopLevelControl* topLevel = GetRootControl();
	return topLevel->GetHostWindow();
}


}	// GUI
