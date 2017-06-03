#include "EngineCore/stdafx.h"
#include "BaseInputController.h"


RTTR_REGISTRATION
{
	rttr::registration::class_< sw::BaseInputController >( "BaseInputController" );
}

namespace sw
{


 //----------------------------------------------------------------------------------------------//
//									BaseInputController
//----------------------------------------------------------------------------------------------//

// ================================ //
//
BaseInputController::BaseInputController( InputAbstractionLayerBase* layer )
	: m_abstractionLayer( layer )
{}

// ================================ //
//
BaseInputController::~BaseInputController() {};


}	// sw

