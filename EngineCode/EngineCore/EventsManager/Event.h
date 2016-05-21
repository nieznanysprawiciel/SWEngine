#pragma once
/**@file Event.h
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.
*/

#include "Common/RTTR.h"




/**@brief @deprecated Zamiast tego u¿yj rttr::type.*/
typedef enum class EventType
{
	KeyDownEvent,
	KeyUpEvent,
	RenderOnceEndedEvent
} EventType;



/**Klasa bazowa dla wszystkich eventów w silniku.*/
class Event
{
	RTTR_ENABLE()
public:
	unsigned int				type;		// @deprecated Zmienna okreœlaj¹ca typ zostanie zast¹piona przez RTTR.

	Event( unsigned int type );
	virtual ~Event() = default;
};

