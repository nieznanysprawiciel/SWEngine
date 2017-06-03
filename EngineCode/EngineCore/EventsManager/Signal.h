#pragma once

#include "swCommonLib/Common/EngineObject.h"
#include "swCommonLib/External/FastDelegate/FastDelegate.h"



namespace sw
{


class Event;


typedef fastdelegate::FastDelegate2< const EngineObject*, Event* >		SignalDelegate;
typedef fastdelegate::FastDelegate2< const EngineObject*, Event* >		EventDelegate;



/**@brief */
struct Signal
{
	const EngineObject*		Sender;
	const EngineObject*		Receiver;
	Event*					Parameters;		///< Event zawieraj¹cy parametry wywo³ania.
	SignalDelegate			SignalHandler;	///< Delegat zawieraj¹cy funkcjê obs³ugi sygna³u.
	SignalDelegate			OnExecuted;		///< Delegat wywo³ywany, gdy zakoñczy siê obs³uga signa³u.
};


/**@brief */
struct EventListener
{
	const EngineObject*		Receiver;
	EventDelegate			EventHandler;
};

}	// sw
