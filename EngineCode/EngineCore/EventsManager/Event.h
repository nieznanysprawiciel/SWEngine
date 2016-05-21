#pragma once
/**@file Event.h
@author nieznanysprawiciel
@copyright Plik jest cz�ci� silnika graficznego SWEngine.
*/

#include "Common/RTTR.h"




/**@brief @deprecated Zamiast tego u�yj rttr::type.*/
typedef enum class EventType
{
	KeyDownEvent,
	KeyUpEvent,
	RenderOnceEndedEvent
} EventType;



/**Klasa bazowa dla wszystkich event�w w silniku.*/
class Event
{
	RTTR_ENABLE()
public:
	unsigned int				type;		// @deprecated Zmienna okre�laj�ca typ zostanie zast�piona przez RTTR.

	Event( unsigned int type );
	virtual ~Event() = default;
};

