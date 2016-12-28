#include "catch.hpp"


#include "Common/Properties/Properties.h"

#include "TestClasses.h"




TEST_CASE( "Property helpers" )
{
	TestAnim		AnimClass;

	std::string someValueNestedPath = "OtherData/Additional/SomeValue";

	auto address = Properties::GetProperty( &AnimClass, someValueNestedPath );

	auto object = address.first;
	auto property = address.second;

	CHECK( object.get_type() == TypeID::get< Nested2* >() );
	CHECK( property.get_type() == TypeID::get< float >() );
	CHECK( property.get_declaring_type() == TypeID::get< Nested2 >() );

	std::string someValuePath = "OtherData/SomeValue";

	address = Properties::GetProperty( &AnimClass, someValuePath );

	object = address.first;
	property = address.second;

	CHECK( object.get_type() == TypeID::get< Nested* >() );
	CHECK( property.get_type() == TypeID::get< float >() );
	CHECK( property.get_declaring_type() == TypeID::get< Nested >() );

	someValuePath = "Energy";

	address = Properties::GetProperty( &AnimClass, someValuePath );

	object = address.first;
	property = address.second;

	CHECK( object.get_type() == TypeID::get< TestAnim* >() );
	CHECK( property.get_type() == TypeID::get< float >() );
	CHECK( property.get_declaring_type() == TypeID::get< TestAnim >() );

	someValuePath = "Fake";

	address = Properties::GetProperty( &AnimClass, someValuePath );

	object = address.first;
	property = address.second;

	CHECK( !object.get_type().is_valid() );
	CHECK( !property.get_type().is_valid() );
	CHECK( !property.get_declaring_type().is_valid() );
}


