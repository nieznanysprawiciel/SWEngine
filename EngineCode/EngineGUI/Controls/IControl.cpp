#include "IControl.h"


namespace GUI
{


/**@brief */
IControl::IControl( IControl* parent )
	:	m_parent( parent )
{ }

/**@brief */
IControl::~IControl()
{ }


}	// GUI
