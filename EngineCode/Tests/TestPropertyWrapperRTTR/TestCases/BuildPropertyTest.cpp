#include "Tests/TestPropertyWrapperRTTR/ExampleClasses/CreateTestActor.h"



#using "PropertyWrapperRTTR.dll" as_friend

using namespace sw::EditorPlugin;
using namespace Microsoft::VisualStudio::TestTools::UnitTesting;




namespace EngineObjectWrapperTest
{


[TestClass]
public ref class BuildPropertiesTestClass
{
public:

	// ================================ //
	// EngineObjectMeta class test for property less object. 
	[TestMethod]
	void		EngineObjectMetaPropertyLessClass()
	{
		auto actor = CreateControllerBase();
		Assert::IsTrue( actor != nullptr, "Shouldn't be null" );

		EngineObjectWrapper^ wrapper = gcnew EngineObjectWrapper( actor );
		bool result = wrapper != nullptr;
		Assert::IsTrue( result );

		EngineObjectMeta^ metaActor = gcnew EngineObjectMeta( wrapper );

		bool creationFailed = metaActor == nullptr;
		Assert::IsFalse( creationFailed );

		Assert::IsTrue( metaActor->Type == wrapper->Type );
		Assert::IsTrue( metaActor->TypeName == wrapper->TypeName );

		Assert::IsTrue( metaActor->Actor == wrapper );

		auto baseClasses = metaActor->BaseClasses;
		Assert::AreEqual( baseClasses->Count, 1 );

		baseClasses[ 0 ] == "EngineObject";

		// This actor shouldn't have properties.
		auto props = metaActor->Properties;
		Assert::AreEqual( props->Count, 0 );
	}


	// ================================ //
	// 
	[TestMethod]
	void		BuildOneLevelHierarchy()
	{
		Actor* actor = CreateSimpleNullActor();
		Assert::IsTrue( actor != nullptr, "Shouldn't be null" );

		EngineObjectWrapper^ wrapper = gcnew EngineObjectWrapper( actor );
		bool result = wrapper != nullptr;
		Assert::IsTrue( result );

		EngineObjectMeta^ metaActor = gcnew EngineObjectMeta( wrapper );

		bool creationFailed = metaActor == nullptr;
		Assert::IsFalse( creationFailed );

		// Properties
		auto props = metaActor->Properties;
		Assert::AreEqual( props->Count, 3 );


	}

};

}

