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



	}


	//// ================================ //
	//// 
	//[TestMethod]
	//void		BuildOneLevelHierarchy()
	//{
	//	Actor* actor = CreateSimpleNullActor();
	//	Assert::IsTrue( actor != nullptr, "Shouldn't be null" );


	//}

};

}

