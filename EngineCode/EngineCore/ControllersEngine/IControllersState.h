#pragma once
/**
@file IControllerState.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/



#include "swCommonLib/Common/EngineObject.h"
#include "EngineCore/Actors/BasicActors/CameraActor.h"


namespace sw
{


class EngineInterface;


/**@brief Klasa przechowuj�ca stan kontroler�w oraz silnika i gry.

Kontrolery dostan� t� klas� w funkcjach @ref IController::ControlObjectPre
i @ref IController::ControlObjectPost. W ten spos�b mog� pobierac wszystkie dane,
kt�re udost�pni im u�ytkownik.

@todo W przysz�o�ci klasa powinna by� implementowana przez u�ytkownika i ustawiana w ControllersEngine.*/
struct IControllersState : public EngineObject
{
	EngineInterface*		Engine;
	CameraData				Camera;

public:
	explicit IControllersState();
	virtual ~IControllersState();
};


}	// sw