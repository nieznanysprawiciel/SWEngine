#include "catch.hpp"

#include "Common/ParameterAnimation/Animation/Animation.h"

#include "Common/ParameterAnimation/Parameters/DirectPropertyh.h"
#include "Common/ParameterAnimation/Parameters/PropertyPath.h"
#include "Common/ParameterAnimation/Parameters/StringPropertyPath.h"


#include <string>



struct Nested2
{
	float			SomeValue;
	int				IntValue;
	std::string		Name;
};

struct Nested
{
	float			SomeValue;
	int				IntValue;
	std::string		Name;
	Nested2			Additional;
};


/**@brief */
class TestAnim
{
	RTTR_REGISTRATION_FRIEND;
private:
	float			m_energy;
	float			m_mass;
	float			m_power;
	float			m_shit;

	Nested			m_otherData;

protected:
public:
	Ptr< FloatAnimation< StringPropertyPath > >		FloatAnimString;
	Ptr< FloatAnimation< PropertyPath > >			FloatAnimProperty;
	Ptr< FloatAnimation< DirectProperty > >			FloatAnimDirect;

public:
	explicit		TestAnim();
	~TestAnim() = default;

};


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


// ================================ //
//
TestAnim::TestAnim()
{}


TEST_CASE( "Animations", "[AnimationClassTest]" )
{
	TestAnim		AnimClass;

	std::string someValueNestedPath = "OtherData/Additional/SomeValue";
	std::string someValuePath = "OtherData/SomeValue";
	std::string energyPropName = "Energy";

	AnimClass.FloatAnimString = MakePtr< FloatAnimation< StringPropertyPath > >( someValueNestedPath );

	// Testing FloatAnimation< StringPropertyPath >
	// =========================================================== //

	// First key is added automatically.
	CHECK( AnimClass.FloatAnimString->GetKey( 0.0 )->Time == 0.0 );
	CHECK( AnimClass.FloatAnimString->GetKey( 0.0 )->Value.Value == 0.0 );




	// Testing FloatAnimation< PropertyPath >
	// =========================================================== //


	AnimClass.FloatAnimProperty = MakePtr< FloatAnimation< PropertyPath > >( &AnimClass, someValuePath );


	// Testing FloatAnimation< DirectProperty >
	// =========================================================== //

	AnimClass.FloatAnimProperty = MakePtr< FloatAnimation< DirectProperty > >( &AnimClass, energyPropName );
}



