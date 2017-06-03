/**
@file Event.cpp
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/


#include "EngineCore/stdafx.h"
#include "Event.h"

#include "EngineCore/EventsManager/Events/KeyDownEvent.h"
#include "EngineCore/EventsManager/Events/KeyUpEvent.h"
#include "EngineCore/EventsManager/Events/RenderOnceEndedEvent.h"


#include "swCommonLib/Common/MemoryLeaks.h"


RTTR_REGISTRATION
{
	rttr::registration::class_< sw::Event >( "Event" );
	rttr::registration::class_< sw::KeyDownEvent >( "KeyDownEvent" );
	rttr::registration::class_< sw::KeyUpEvent >( "KeyUpEvent" );
	rttr::registration::class_< sw::RenderOnceEndedEvent >( "RenderOnceEndedEvent" );
}


namespace sw
{


//=================================================================//
//								Event
//=================================================================//
Event::Event( unsigned int type )
	: type( type )
{}


//=================================================================//
//						KeyDownEvent
//=================================================================//

KeyDownEvent::KeyDownEvent( short v_index )
	: Event( (unsigned int)EventType::KeyDownEvent )
{
	VirtualIndex = v_index;
}



//=================================================================//
//						KeyUpEvent
//=================================================================//
KeyUpEvent::KeyUpEvent( short v_index )
	: Event( (unsigned int)EventType::KeyUpEvent )
{
	VirtualIndex = v_index;
}


//=================================================================//
//						RenderOnceEndedEvent
//=================================================================//
RenderOnceEndedEvent::RenderOnceEndedEvent()
	: Event( (unsigned int)EventType::RenderOnceEndedEvent )
{}


}	// sw

