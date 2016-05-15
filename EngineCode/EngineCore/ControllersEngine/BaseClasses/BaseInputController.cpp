#include "EngineCore/stdafx.h"
#include "BaseInputController.h"


RTTR_REGISTRATION
{
	rttr::registration::class_< BaseInputController >( "BaseInputController" );
}

//----------------------------------------------------------------------------------------------//
//									BaseInputController
//----------------------------------------------------------------------------------------------//

BaseInputController::BaseInputController( InputAbstractionLayerBase* layer )
: m_abstractionLayer( layer )
{}

BaseInputController::~BaseInputController(){};

