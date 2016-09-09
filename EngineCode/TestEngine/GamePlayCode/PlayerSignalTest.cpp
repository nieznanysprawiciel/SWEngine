#include "PlayerSignalTest.h"


RTTR_REGISTRATION
{

	rttr::registration::enumeration< NestedFunctionType >( "NestedFunctionType" )
	(
		rttr::value( "Known", NestedFunctionType::Known ),
		rttr::value( "Unknown", NestedFunctionType::Unknown )
	);

	rttr::registration::class_< NestedStruct >( "NestedStruct" )
		.property( "Data1", &NestedStruct::Data1 )
		.property( "Data2", &NestedStruct::Data2 );

	rttr::registration::class_< PlayerSignalTest >( "PlayerSignalTest" )
		.property( "TestValue", &PlayerSignalTest::m_testValue )
		( 
			rttr::metadata( MetaDataType::AllowInSaveFile, false),
			rttr::metadata( MetaDataType::Serialize, false )
		)
		.property( "FunctionType", &PlayerSignalTest::m_funType )
		.property( "NestedStruct", &PlayerSignalTest::m_struct ) BIND_AS_PTR;

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
