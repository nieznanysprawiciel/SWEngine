#include "PlayerSignalTest.h"
#include "Objects/GenericActor.h"



RTTR_REGISTRATION
{

	rttr::registration::enumeration< NestedFunctionType >( "NestedFunctionType" )
	(
		rttr::value( "Known", NestedFunctionType::Known ),
		rttr::value( "Unknown", NestedFunctionType::Unknown )
	);

	rttr::registration::class_< NestedNestedStruct >( "NestedNestedStruct" )
		.property( "Value1", &NestedNestedStruct::Value1 )
		.property( "Value2", &NestedNestedStruct::Value2 );

	rttr::registration::class_< NestedStruct >( "NestedStruct" )
		.property( "Data1", &NestedStruct::Data1 )
		.property( "Data2", &NestedStruct::Data2 )
		.property( "Additional", &NestedStruct::Additional ) BIND_AS_PTR;

	rttr::registration::class_< std::vector< NestedStruct > >( "std::vector< NestedStruct >" );
	rttr::registration::class_< std::vector< EngineObject* > >( "std::vector< EngineObject* >" );
	

	rttr::registration::class_< PlayerSignalTest >( "PlayerSignalTest" )
		.property( "TestValue", &PlayerSignalTest::m_testValue )
		( 
			rttr::metadata( MetaDataType::AllowInSaveFile, false),
			rttr::metadata( MetaDataType::Serialize, false )
		)
		.property( "FunctionType", &PlayerSignalTest::m_funType )
		.property( "DataData", &PlayerSignalTest::m_struct ) BIND_AS_PTR
		.property( "OtherData", &PlayerSignalTest::m_otherData )
		.property( "Generics", &PlayerSignalTest::m_generics );

}


PlayerSignalTest::PlayerSignalTest()
{
	const int arraySize = 5;
	const int genericArraySize = 3;

	m_testValue = 0;

	srand( arraySize );
	
	for( int i = 0; i < arraySize; ++i )
	{
		NestedStruct struct1;
		struct1.Data1 = rand();
		struct1.Data2 = rand();
		struct1.Additional.Value1 = (float)rand();
		struct1.Additional.Value2 = (double)rand();

		m_otherData.push_back( struct1 );
	}

	m_generics.push_back( new GenericActor() );
	m_generics.push_back( new GenericActor() );
	m_generics.push_back( new GenericObject() );
}


PlayerSignalTest::~PlayerSignalTest()
{
	for( auto ptr : m_generics )
		delete ptr;
}

/**@brief */
void	PlayerSignalTest::DoYourJob		( const EngineObject* sender, sw::Event* params )
{
	m_testValue += static_cast< EventData* >( params )->Data;
}

void	PlayerSignalTest::InitJob		( const EngineObject* sender, sw::Event* keyEvent )
{
	m_otherPlayer.SendSignal< &PlayerSignalTest::DoYourJob >( this, new EventData );

	const sw::ActorPtr< sw::ActorBase > actor = m_otherPlayer.DynamicCast< sw::ActorBase >();
	m_otherPlayer.StaticCast< sw::ActorBase >();

	actor.StaticCast< const sw::StaticActor >();
	actor.DynamicCast< const sw::StaticActor >();
}
