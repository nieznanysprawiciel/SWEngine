#include "PlayerSignalTest.h"


RTTR_REGISTRATION
{
	rttr::registration::class_< PlayerSignalTest >( "PlayerSignalTest" );
}


PlayerSignalTest::PlayerSignalTest()
{
	m_testValue = 0;
}


PlayerSignalTest::~PlayerSignalTest()
{ }

/**@brief */
void	PlayerSignalTest::DoYourJob		( const EngineObject* sender, Event* params )
{
	m_testValue += static_cast< EventData* >( params )->Data;
}

void	PlayerSignalTest::InitJob		( const EngineObject* sender, Event* keyEvent )
{
	m_otherPlayer.SendSignal< &PlayerSignalTest::DoYourJob >( this, new EventData );

	const ActorPtr< ActorBase > actor = m_otherPlayer.DynamicCast< ActorBase >();
	m_otherPlayer.StaticCast< ActorBase >();

	actor.StaticCast< const StaticActor >();
	actor.DynamicCast< const StaticActor >();
}
