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

	EngineObject* object = actor;

	EngineObjectWrapper^ wrapper = gcnew EngineObjectWrapper( object );
	//bool result = wrapper != nullptr;
	//REQUIRE( result );

	//// GetActorPtr returns wrapped pointer casted to int.
	//CHECK( wrapper->GetActorPtr().ToInt64() == (int64)actor );

	//// GetTypeName returns the same name as rttr::type.
	//System::String^ typeName = gcnew System::String( actor->get_type().get_name().to_string().c_str() );
	//CHECK( typeName == wrapper->GetTypeName() );
	//CHECK( typeName == wrapper->TypeName );

	//CHECK( wrapper->GetPtr() == actor );
	//CHECK( wrapper->GetTypeID() == actor->get_type() );
	//CHECK( actor->get_type().get_id() == wrapper->Type );

	//auto instance = wrapper->GetInstance();

	//CHECK( instance.is_valid() );
	//CHECK( instance.get_derived_type() == actor->get_type() );
	//CHECK( instance.get_type() == actor->get_type() );
	//CHECK( !instance.get_wrapped_instance().is_valid() );
	

}