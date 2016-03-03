#include "EngineCore/stdafx.h"
#include "Event.h"


#include "Common/MemoryLeaks.h"

//=================================================================//
//								Event
//=================================================================//
Event::Event( unsigned int type )
	: type( type )
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



//=================================================================//
//						KeyUpEvent
//=================================================================//
KeyUpEvent::KeyUpEvent(short v_index)
: Event( (unsigned int)EventType::KeyUpEvent )
{
	virtual_index = v_index;
}


//=================================================================//
//						RenderOnceEndedEvent
//=================================================================//
RenderOnceEndedEvent::RenderOnceEndedEvent()
: Event( (unsigned int)EventType::RenderOnceEndedEvent )
{
}