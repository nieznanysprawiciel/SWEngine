
#include "Tests/TestPropertyWrapperRTTR/ExampleClasses/CreateTestActor.h"




#using "PropertyWrapperRTTR.dll" as_friend


using namespace sw::EditorPlugin;
using namespace Microsoft::VisualStudio::TestTools::UnitTesting;



namespace EngineObjectWrapperTest
{


[TestClass]
public ref class EngineObjectWrapperTestClass
{
public:


	// ================================ //
	// Creates EngineObjectWrapper and checks function result types.
	[TestMethod]
	void		CreateSimpleActor()
	//TEST_METHOD( "Create Simple Actor", "[EngineObjectWrapper]" )
	{
		Actor* actor = CreateSimpleNullActor();
		Assert::IsTrue( actor != nullptr, "Shouldn't be null" );

		EngineObject* object = actor;

		EngineObjectWrapper^ wrapper = gcnew EngineObjectWrapper( object );
		bool result = wrapper != nullptr;
		Assert::IsTrue( result );

		// GetActorPtr returns wrapped pointer casted to int.
		Assert::IsTrue( wrapper->GetActorPtr().ToInt64() == (int64)actor );

		// GetTypeName returns the same name as rttr::type.
		System::String^ typeName = gcnew System::String( actor->get_type().get_name().to_string().c_str() );
		Assert::IsTrue( typeName == wrapper->GetTypeName() );
		Assert::IsTrue( typeName == wrapper->TypeName );

		Assert::IsTrue( wrapper->GetPtr() == actor );
		Assert::IsTrue( wrapper->GetTypeID() == actor->get_type() );
		Assert::IsTrue( actor->get_type().get_id() == wrapper->Type );

	}


	// ================================ //
	// Created Derived object and checks results
	[TestMethod]
	void		CreateDerivedActor()
	{
		auto actor = CreateDerivedNullActor();
		Assert::IsTrue( actor != nullptr, "Shouldn't be null" );

		EngineObjectWrapper^ wrapper = gcnew EngineObjectWrapper( actor );
		bool result = wrapper != nullptr;
		Assert::IsTrue( result );

		System::String^ typeName = gcnew System::String( actor->get_type().get_name().to_string().c_str() );
		Assert::IsTrue( typeName == wrapper->GetTypeName() );
		Assert::IsTrue( typeName == wrapper->TypeName );

		Assert::IsTrue( wrapper->GetTypeID() == actor->get_type() );
	}

};


}
