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
	CHECK( AnimClass.FloatAnimString->GetKey( 0.0 )->Param.Value == 7.0f );




	// Testing FloatAnimation< PropertyPath >
	// =========================================================== //
	std::string someValuePath = "OtherData/SomeValue";
	

	AnimClass.FloatAnimProperty = MakePtr< FloatAnimation< PropertyPath > >( &AnimClass, someValuePath );


	// Testing FloatAnimation< DirectProperty >
	// =========================================================== //
	std::string energyPropName = "Energy";

	AnimClass.FloatAnimDirect = MakePtr< FloatAnimation< DirectProperty > >( &AnimClass, energyPropName );
}



