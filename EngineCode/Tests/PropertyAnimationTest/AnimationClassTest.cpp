#include "catch.hpp"

#include "Common/ParameterAnimation/Animation/Animation.h"

#include "Common/ParameterAnimation/Parameters/DirectPropertyh.h"
#include "Common/ParameterAnimation/Parameters/PropertyPath.h"
#include "Common/ParameterAnimation/Parameters/StringPropertyPath.h"

#include "TestClasses.h"

#include <string>



RTTR_REGISTRATION
{
	rttr::registration::class_< Nested2 >( "Nested2" )
		.property( "SomeValue", &Nested2::SomeValue )
		.property( "IntValue", &Nested2::IntValue )
		.property( "Name", &Nested2::Name );
	
	rttr::registration::class_< Nested >( "Nested" )
		.property( "SomeValue", &Nested::SomeValue )
		.property( "IntValue", &Nested::IntValue )
		.property( "Name", &Nested::Name )
		.property( "Additional", &Nested::Additional ) BIND_AS_PTR;

	rttr::registration::class_< TestAnim >( "TestAnim" )
		.property( "Energy", &TestAnim::m_energy )
		.property( "Mass", &TestAnim::m_mass )
		.property( "Power", &TestAnim::m_power )
		.property( "Shit", &TestAnim::m_shit )
		.property( "OtherData", &TestAnim::m_otherData ) BIND_AS_PTR;
}



TEST_CASE( "Animations", "[AnimationClassTest]" )
{
	TestAnim		AnimClass;

	std::string someValueNestedPath = "OtherData/Additional/SomeValue";
	AnimClass.FloatAnimString = MakePtr< FloatAnimation< StringPropertyPath > >( &AnimClass, someValueNestedPath );

	// Testing FloatAnimation< StringPropertyPath >
	// =========================================================== //

	// First key is added automatically.
	CHECK( AnimClass.FloatAnimString->GetKey( 0.0 )->Time == 0.0 );
	CHECK( AnimClass.FloatAnimString->GetKey( 0.0 )->Value == 7.0f );

// Add new keys.
	CHECK( AnimClass.FloatAnimString->AddKey( 2.0, 3.0f ) );
	CHECK( AnimClass.FloatAnimString->AddKey( 3.0, 0.0f ) );
	CHECK( AnimClass.FloatAnimString->AddKey( 4.0, 1.0f ) );
	CHECK( AnimClass.FloatAnimString->AddKey( 5.0, 0.0f ) );

	// Check new added keys.
	CHECK( AnimClass.FloatAnimString->GetKey( 0.0 )->Value == 7.0f );
	CHECK( AnimClass.FloatAnimString->GetKey( 2.0 )->Value == 3.0f );
	CHECK( AnimClass.FloatAnimString->GetKey( 3.0 )->Value == 0.0f );
	CHECK( AnimClass.FloatAnimString->GetKey( 4.0 )->Value == 1.0f );
	CHECK( AnimClass.FloatAnimString->GetKey( 5.0 )->Value == 0.0f );

// Add key in existing keyframe.
	CHECK( !AnimClass.FloatAnimString->AddKey( 4.0, 2.0f ) );
	CHECK( AnimClass.FloatAnimString->GetKey( 4.0 )->Value == 2.0f );

// Add key between existing keys.
	CHECK( AnimClass.FloatAnimString->AddKey( 1.5, 6.0f ) );
	CHECK( AnimClass.FloatAnimString->AddKey( 3.5, 2.0f ) );
	CHECK( AnimClass.FloatAnimString->AddKey( 4.5, -1.0f ) );

	CHECK( AnimClass.FloatAnimString->GetKey( 1.5 )->Value == 6.0f );
	CHECK( AnimClass.FloatAnimString->GetKey( 3.5 )->Value == 2.0f );
	CHECK( AnimClass.FloatAnimString->GetKey( 4.5 )->Value == -1.0f );

// Remove keys
	CHECK( AnimClass.FloatAnimString->RemoveKey( 1.5 ) );
	CHECK( AnimClass.FloatAnimString->RemoveKey( 3.5 ) );
	CHECK( AnimClass.FloatAnimString->RemoveKey( 4.5 ) );

	CHECK( AnimClass.FloatAnimString->GetKey( 1.5 ) == nullptr );
	CHECK( AnimClass.FloatAnimString->GetKey( 3.5 ) == nullptr );
	CHECK( AnimClass.FloatAnimString->GetKey( 4.5 ) == nullptr );

// Update keys
	CHECK( AnimClass.FloatAnimString->UpdateKey( 2.0, 4.0f ) );
	CHECK( AnimClass.FloatAnimString->UpdateKey( 3.0, 1.0f ) );
	CHECK( AnimClass.FloatAnimString->UpdateKey( 4.0, -3.0f ) );
	CHECK( AnimClass.FloatAnimString->UpdateKey( 5.0, 2.0f ) );

	CHECK( AnimClass.FloatAnimString->GetKey( 2.0 )->Value == 4.0f );
	CHECK( AnimClass.FloatAnimString->GetKey( 3.0 )->Value == 1.0f );
	CHECK( AnimClass.FloatAnimString->GetKey( 4.0 )->Value == -3.0f );
	CHECK( AnimClass.FloatAnimString->GetKey( 5.0 )->Value == 2.0f );


	// Testing FloatAnimation< PropertyPath >
	// =========================================================== //
	std::string someValuePath = "OtherData/SomeValue";
	AnimClass.FloatAnimProperty = MakePtr< FloatAnimation< PropertyPath > >( &AnimClass, someValuePath );

	CHECK( AnimClass.FloatAnimProperty->GetKey( 0.0 )->Time == 0.0 );
	CHECK( AnimClass.FloatAnimProperty->GetKey( 0.0 )->Value == 5.0f );





	// Testing FloatAnimation< DirectProperty >
	// =========================================================== //
	std::string energyPropName = "Energy";
	AnimClass.FloatAnimDirect = MakePtr< FloatAnimation< DirectProperty > >( &AnimClass, energyPropName );

	CHECK( AnimClass.FloatAnimDirect->GetKey( 0.0 )->Time == 0.0 );
	CHECK( AnimClass.FloatAnimDirect->GetKey( 0.0 )->Value == 1.0f );





}



