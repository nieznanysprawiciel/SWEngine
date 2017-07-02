#include "swCommonLib/External/Catch/catch.hpp"


#include "Tests/TestPropertyWrapper/ExampleClasses/CreateTestActor.h"


#using "PropertyWrapperRTTR.dll" as_friend
using namespace sw::EditorPlugin;


// ================================ //
// Creates EngineObjectWrapper and checks function result types.
TEST_CASE( "Create Simple Actor", "[EngineObjectWrapper]" )
{
	Actor* actor = CreateSimpleNullActor();
	REQUIRE( actor != __nullptr );

	EngineObjectWrapper^ wrapper = gcnew EngineObjectWrapper( actor );
	bool result = wrapper != nullptr;
	REQUIRE( result );

}