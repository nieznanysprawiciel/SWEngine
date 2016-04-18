#include "EngineCore/stdafx.h"
/**@file basic_interfaces.cpp
@brief Plik zawiera definicje metod klas z pliku basic_interfaces.h
*/

#include "ActorObjects.h"

#include "Common/MemoryLeaks.h"


//----------------------------------------------------------------------------------------------//
//									BaseInputController
//----------------------------------------------------------------------------------------------//

BaseInputController::BaseInputController( InputAbstractionLayerBase* layer )
: abstraction_layer( layer )
{}

BaseInputController::~BaseInputController(){};


