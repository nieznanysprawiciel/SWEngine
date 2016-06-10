#pragma once

#include "Common/EngineObject.h"
#include "Common/Delegate.h"


/**@defgroup EngineGUI
@ingroup ModulesStructure
@brief Modu³ s³u¿¹cy do renderowania interfejsu graficznego u¿ytkownika.*/


namespace GUI
{

/**@brief Klasa bazowa dla kontrolek.*/
class IControl : public EngineObject
{
	RTTR_ENABLE( EngineObject )
	RTTR_REGISTRATION_FRIEND
private:
protected:

	IControl*			m_parent;

public:
					IControl	( IControl* parent );
	virtual			~IControl	();

	IControl*		GetParent	()			{ return m_parent; }




};

}	// GUI
