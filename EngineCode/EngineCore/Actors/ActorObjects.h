#pragma once

/**
@file basic_interfaces.h
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.

@brief Zawiera deklaracje klas bazowych dla obiektów(aktorów):

@ref ActorBase
@ref StaticActor
@ref DynamicActor
@ref CollisionActor
@ref PhysicalActor
@ref DynamicMeshActor
@ref AnimationActor
@ref Controller
@ref BaseAIController
@ref BaseInputController
@ref CameraActor
@ref PathNode
@ref SpawnPoint
*/

class DynamicActor;
class InputAbstractionLayerBase;

#include "BasicActors/ActorBase.h"
#include "BasicActors/StaticActor.h"
#include "BasicActors/DynamicActor.h"
#include "BasicActors/CollisionActor.h"
#include "BasicActors/PhysicalActor.h"
#include "BasicActors/DynamicMeshActor.h"
#include "BasicActors/AnimationActor.h"
#include "BasicActors/CameraActor.h"
#include "BasicActors/ComplexActor.h"
#include "BasicActors/PathNode.h"
#include "BasicActors/SpawnPoint.h"


/**@brief Klasa bazowa dla wszystkich kontrolerów dla obiektów.*/
class Controller
{
public:
	virtual ~Controller() = default;
	virtual void ControlObject( DynamicActor* ) = 0;
};

/**@brief Klasa bazowa dla wszystkich kontrolerów sztucznej inteligencji.*/
class BaseAIController : public Controller
{

	virtual void ControlObject( DynamicActor* ) = 0;
};

/**@brief Klasa bazowa dla wszystkich kontrolerów do sterowania przez u¿ytkownika.*/
class BaseInputController : public Controller
{
protected:
	InputAbstractionLayerBase*		abstraction_layer;

public:
	BaseInputController( InputAbstractionLayerBase* layer );
	virtual ~BaseInputController();

	void SetAbstractionLayer( InputAbstractionLayerBase* layer ) { abstraction_layer = layer; };

	virtual void ControlObject( DynamicActor* ) = 0;
};

