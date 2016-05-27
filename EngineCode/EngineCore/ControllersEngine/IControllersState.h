#pragma once

#include "Common/EngineObject.h"

class EngineInterface;


/**@brief Klasa przechowuj¹ca stan kontrolerów oraz silnika i gry.

Kontrolery dostan¹ tê klasê w funkcjach @ref IController::ControlObjectPre
i @ref IController::ControlObjectPost. W ten sposób mog¹ pobierac wszystkie dane,
które udostêpni im u¿ytkownik.

@todo W przysz³oœci klasa powinna byæ implementowana przez u¿ytkownika i ustawiana w ControllersEngine.*/
struct IControllersState : public EngineObject
{
	EngineInterface*		Engine;


public:
	explicit IControllersState();
	virtual ~IControllersState();
};

