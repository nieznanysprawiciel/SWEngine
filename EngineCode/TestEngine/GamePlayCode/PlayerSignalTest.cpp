#include "PlayerSignalTest.h"


RTTR_REGISTRATION
{
	rttr::registration::class_< PlayerSignalTest >( "PlayerSignalTest" );
}


PlayerSignalTest::PlayerSignalTest()
{ }


PlayerSignalTest::~PlayerSignalTest()
{ }

/**@brief */
void PlayerSignalTest::DoYourJob()
{
	m_otherPlayer.Signal< &PlayerSignalTest::DoYourJob >( this, new EventData );
}
