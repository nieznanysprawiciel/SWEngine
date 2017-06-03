#pragma once

#include "swCommonLib/Common/RTTR.h"
#include "EngineCore/Actors/ActorObjects.h"
#include "EngineCore/Actors/ActorPointer.h"

#include "EngineCore/EventsManager/Event.h"

#include <vector>


// ================================ //
//
struct EventData : public Event
{
	int		Data;

	EventData()
		:	Event( 4 )	// shit
	{
		Data = 1;
	}
};

// ================================ //
//
enum NestedFunctionType
{
	Unknown,
	Known
};

struct NestedNestedStruct
{
	float		Value1;
	double		Value2;
};


struct NestedStruct
{
	int		Data1;
	int		Data2;

	NestedNestedStruct		Additional;
};



class PlayerSignalTest	: public DynamicActor
{
	RTTR_ENABLE( DynamicActor );
	RTTR_REGISTRATION_FRIEND;
private:

	ActorPtr< PlayerSignalTest >		m_otherPlayer;
	
	int									m_testValue;
	NestedStruct						m_struct;
	NestedFunctionType					m_funType;

	std::vector< NestedStruct >			m_otherData;
	std::vector< EngineObject* >		m_generics;

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

