#include "EngineCore/stdafx.h"
#include "Event.h"


#include "Common\memory_leaks.h"

//=================================================================//
//								Event
//=================================================================//
Event::Event( unsigned int type )
	: type( type )
{
}


Event::~Event()
{
}

//=================================================================//
//						KeyDownEvent
//=================================================================//

KeyDownEvent::KeyDownEvent(short v_index)
: Event( (unsigned int)EventType::KeyDownEvent )
{
	virtual_index = v_index;
}

KeyDownEvent::~KeyDownEvent()
{
}


//=================================================================//
//						KeyUpEvent
//=================================================================//
KeyUpEvent::KeyUpEvent(short v_index)
: Event( (unsigned int)EventType::KeyUpEvent )
{
	virtual_index = v_index;
}

KeyUpEvent::~KeyUpEvent()
{
}
