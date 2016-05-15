#pragma once
/**@file IController.h
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.

@brief Klasa bazowa dla kontrolerów.*/

#include "Common/EngineObject.h"

class DynamicActor;

/**@brief Klasa bazowa dla wszystkich kontrolerów dla obiektów.*/
class IController : public EngineObject
{
	RTTR_ENABLE( EngineObject )
public:
	virtual ~IController() = default;

	virtual void		ControlObjectPre		( DynamicActor* ) = 0;
	virtual void		ControlObjectPost		( DynamicActor* ) = 0;
};
