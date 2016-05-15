#pragma once
/**@file IController.h
@author nieznanysprawiciel
@copyright Plik jest cz�ci� silnika graficznego SWEngine.

@brief Klasa bazowa dla kontroler�w.*/

#include "Common/EngineObject.h"

class DynamicActor;

/**@brief Klasa bazowa dla wszystkich kontroler�w dla obiekt�w.*/
class IController : public EngineObject
{
	RTTR_ENABLE( EngineObject )
public:
	virtual ~IController() = default;

	virtual void		ControlObjectPre		( DynamicActor* ) = 0;
	virtual void		ControlObjectPost		( DynamicActor* ) = 0;
};
