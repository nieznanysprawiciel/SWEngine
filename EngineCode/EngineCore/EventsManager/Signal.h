#pragma once

#include "FastDelegate.h"

typedef fastdelegate::FastDelegate1<Event*> SignalDelegate;

class Event;
class ActorBase;


struct Signal
{
	const ActorBase*		Sender;
	const ActorBase*		Receiver;
	Event*					Parameters;
	SignalDelegate			Callback;
};

