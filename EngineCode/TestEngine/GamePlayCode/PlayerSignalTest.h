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
		Data = 4;
	}
};


class PlayerSignalTest	: public DynamicMeshActor
{
	RTTR_ENABLE( DynamicMeshActor );
private:

	ActorPtr< PlayerSignalTest >		m_otherPlayer;

public:
	PlayerSignalTest();
	~PlayerSignalTest();

	void		SetOtherPlayer		( PlayerSignalTest* other )
	{
		m_otherPlayer = other;
	}

	void		DoYourJob			();

	static ActorBase*			Create			()	{ return new PlayerSignalTest; }
};

