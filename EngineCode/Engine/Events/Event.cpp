#include "stdafx.h"
#include "Event.h"


#include "memory_leaks.h"

//=================================================================//
//								Event
//=================================================================//
Event::Event(int type)
	: type(type)
{
}


Event::~Event()
{
}

//=================================================================//
//						KeyDownEvent
//=================================================================//

KeyDownEvent::KeyDownEvent(short v_index)
: Event(EVENT_TYPE::KEYDOWNEVENT)
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
: Event(EVENT_TYPE::KEYUPEVENT)
{
	virtual_index = v_index;
}

KeyUpEvent::~KeyUpEvent()
{
}
