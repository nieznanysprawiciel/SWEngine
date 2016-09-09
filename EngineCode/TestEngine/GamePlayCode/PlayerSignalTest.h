#pragma once

#include "Common/RTTR.h"
#include "EngineCore/Actors/ActorObjects.h"
#include "EngineCore/Actors/ActorPointer.h"

#include "EngineCore/EventsManager/Event.h"

struct EventData : public Event
{
	int		Data;

	EventData()
		:	Event( 4 )	// shit
	{
		Data = 1;
	}
};

struct NestedStruct
{
	int		Data1;
	int		Data2;
};


class PlayerSignalTest	: public DynamicActor
{
	RTTR_ENABLE( DynamicActor );
private:

	ActorPtr< PlayerSignalTest >		m_otherPlayer;
	
	int									m_testValue;
	NestedStruct						m_struct;

public:
	PlayerSignalTest();
	~PlayerSignalTest();

	void		SetOtherPlayer		( PlayerSignalTest* other )
	{
		m_otherPlayer = other;
	}

	void		DoYourJob			( const EngineObject* sender, Event* params );
	void		InitJob				( const EngineObject* sender, Event* keyEvent );

	static ActorBase*			Create			()	{ return new PlayerSignalTest; }
};

