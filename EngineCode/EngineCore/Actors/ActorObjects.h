#pragma once

/**
@file basic_interfaces.h
@author nieznanysprawiciel
@copyright Plik jest cz�ci� silnika graficznego SWEngine.

@brief Zawiera deklaracje klas bazowych dla obiekt�w(aktor�w):

EngineObject
Static_object
DynamicActor
Collision_object
PhysicalActor
DynamicMeshActor
AnimationActor
Controller
Standard_AI_controller
Standard_input_controller
CameraActor
*/

class DynamicActor;
class InputAbstractionLayer_base;

#include "BasicActors/EngineObject.h"
#include "BasicActors/StaticActor.h"
#include "BasicActors/DynamicActor.h"
#include "BasicActors/CollisionActor.h"
#include "BasicActors/PhysicalActor.h"
#include "BasicActors/PhysicalActor.h"
#include "BasicActors/DynamicMeshActor.h"
#include "BasicActors/AnimationActor.h"
#include "BasicActors/CameraActor.h"
#include "BasicActors/ComplexActor.h"


/**@brief Klasa bazowa dla wszystkich kontroler�w dla obiekt�w.*/
class Controller
{
public:
	virtual ~Controller() = default;
	virtual void ControlObject( DynamicActor* ) = 0;
};

/**@brief Klasa bazowa dla wszystkich kontroler�w sztucznej inteligencji.*/
class BaseAIController : public Controller
{

	virtual void ControlObject( DynamicActor* ) = 0;
};

/**@brief Klasa bazowa dla wszystkich kontroler�w do sterowania przez u�ytkownika.*/
class BaseInputController : public Controller
{
protected:
	InputAbstractionLayer_base*		abstraction_layer;

public:
	BaseInputController( InputAbstractionLayer_base* layer );
	virtual ~BaseInputController();

	void SetAbstractionLayer( InputAbstractionLayer_base* layer ) { abstraction_layer = layer; };

	virtual void ControlObject( DynamicActor* ) = 0;
};

