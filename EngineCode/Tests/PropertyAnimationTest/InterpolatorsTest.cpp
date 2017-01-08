#include "catch.hpp"

#include "Common/ParameterAnimation/Animation/Animation.h"

#include "Common/ParameterAnimation/Parameters/DirectPropertyh.h"
#include "Common/ParameterAnimation/Parameters/PropertyPath.h"
#include "Common/ParameterAnimation/Parameters/StringPropertyPath.h"

#include "TestClasses.h"

#include <string>


RTTR_REGISTRATION
{
	rttr::registration::enumeration< Methods >( "Methods" )
	(
		rttr::value( "Discrete", Methods::Discrete ),
		rttr::value( "Integral", Methods::Integral ),
		rttr::value( "Linear", Methods::Linear )
	);

	rttr::registration::class_< TestInterpolators >( "TestAnim" )
		.property( "DoubleField", &TestInterpolators::DoubleField )
		.property( "FloatField", &TestInterpolators::FloatField )
		.property( "UIntField64", &TestInterpolators::UIntField64 )
		.property( "IntField64", &TestInterpolators::IntField64 )
		.property( "UIntField32", &TestInterpolators::UIntField32 )
		.property( "IntField32", &TestInterpolators::IntField32 )
		.property( "UIntField16", &TestInterpolators::UIntField16 )
		.property( "IntField16", &TestInterpolators::IntField16 )
		.property( "UIntField8", &TestInterpolators::UIntField8 )
		.property( "IntField8", &TestInterpolators::IntField8 )
		.property( "CharField", &TestInterpolators::CharField )
		.property( "BoolField", &TestInterpolators::BoolField )
		.property( "StringField", &TestInterpolators::StringField )
		.property( "WStringField", &TestInterpolators::WStringField )
		.property( "EnumField", &TestInterpolators::EnumField );
}


// ================================ //
//
template< typename PropType >
void			TestProperty		( TestInterpolators& animClass, const std::string& propertyPath )
{
	SECTION( propertyPath.c_str() )
	{
		Ptr< AnimationImpl< PropType, PropertyPath< PropType > > > animator = MakePtr< AnimationImpl< PropType, PropertyPath< PropType > > >( &animClass, propertyPath );

		CHECK( animator->GetKey( 0.0 )->Value == 4 );

		// Add new keys.
		CHECK( animator->AddKey( 2.0, 8 ) );
		CHECK( animator->AddKey( 3.0, 5 ) );
		CHECK( animator->AddKey( 4.0, 1 ) );
		CHECK( animator->AddKey( 5.0, 0 ) );

		// Check new added keys.
		CHECK( animator->GetKey( 0.0 )->Value == 4 );
		CHECK( animator->GetKey( 2.0 )->Value == 8 );
		CHECK( animator->GetKey( 3.0 )->Value == 5 );
		CHECK( animator->GetKey( 4.0 )->Value == 1 );
		CHECK( animator->GetKey( 5.0 )->Value == 0 );


		//
		animator->Animate( &animClass, 0.0f );
		CHECK( animClass.GetField< PropType >() == 4 );

		animator->Animate( &animClass, 0.6f );
		CHECK( animClass.GetField< PropType >() == 5 );

		animator->Animate( &animClass, 1.01f );
		CHECK( animClass.GetField< PropType >() == 6 );

		animator->Animate( &animClass, 1.53f );
		CHECK( animClass.GetField< PropType >() == 7 );

		animator->Animate( &animClass, 2.0f );
		CHECK( animClass.GetField< PropType >() == 8 );

		animator->Animate( &animClass, 2.5f );
		CHECK( animClass.GetField< PropType >() == 6 );

		animator->Animate( &animClass, 3.0f );
		CHECK( animClass.GetField< PropType >() == 5 );

		animator->Animate( &animClass, 3.5f );
		CHECK( animClass.GetField< PropType >() == 3 );

		animator->Animate( &animClass, 4.0f );
		CHECK( animClass.GetField< PropType >() == 1 );

		animator->Animate( &animClass, 5.0f );
		CHECK( animClass.GetField< PropType >() == 0 );

		animator->Animate( &animClass, 6.0f );
		CHECK( animClass.GetField< PropType >() == 0 );
	}

}


template< typename PropType, Size ListSize >
void			TestPropertyDiscrete	( TestInterpolators& animClass, const std::string& propertyPath, PropType (&valuesList)[ ListSize ], TimeType (&timeList)[ ListSize ] )
{
	SECTION( propertyPath.c_str() )
	{
		Ptr< AnimationImpl< PropType, PropertyPath< PropType > > > animator = MakePtr< AnimationImpl< PropType, PropertyPath< PropType > > >( &animClass, propertyPath );

		Size init = 0;
		if( timeList[ init ] == 0.0f )
		{
			// Should be false !!
			CHECK( !animator->AddKey( timeList[ init ], valuesList[ init ] ) );
			init = 1;
		}

		for( ; init < ListSize; ++init )
		{
			INFO( "Adding keys, key time : " << timeList[ init ] );
			CHECK( animator->AddKey( timeList[ init ], valuesList[ init ] ) );
		}

		REQUIRE( animator->GetKeySet().Keys.size() == ListSize );
		REQUIRE( animator->GetKeySet().Interpolators.size() == ListSize - 1 );

		for( Size i = 0; i < ListSize; ++i )
		{
			INFO( "Checking keys, key time: " << timeList[ i ] );
			CHECK( animator->GetKey( timeList[ i ] )->Value == valuesList[ i ] );
		}

		int divisions = 10;

		for( Size i = 0; i < ListSize - 1; ++i )
		{
			TimeType timeInterval = timeList[ i + 1 ] - timeList[ i ];
								
			for( Size j = 0; j < divisions; ++ j )
			{
				TimeType curTime = timeList[ i ] + j * timeInterval / divisions;
				INFO( "Checking animation, time: " << curTime );

				animator->Animate( &animClass, curTime );
				CHECK( animClass.GetField< PropType >() == valuesList[ i ] );
			}
		}

	}
}



TEST_CASE( "Interpolators", "[Interpolators Types]" )
{
	TestInterpolators		AnimClass;

	TestProperty< uint32 >( AnimClass, "UIntField32" );
	TestProperty< int32 >( AnimClass, "IntField32" );
	TestProperty< uint64 >( AnimClass, "UIntField64" );
	TestProperty< int64 >( AnimClass, "IntField64" );
	TestProperty< uint16 >( AnimClass, "UIntField16" );
	TestProperty< int16 >( AnimClass, "IntField16" );
	TestProperty< uint8 >( AnimClass, "UIntField8" );
	TestProperty< int8 >( AnimClass, "IntField8" );

	{
		std::string valuesArray[] = { "Value 1", "Something important", "Something not important", "Interesting message", "Not interesting message" };
		TimeType timeArray[] ={ 0.0f, 3.0f, 5.0f, 6.1f, 11.0f };

		TestPropertyDiscrete< std::string >( AnimClass, "StringField", valuesArray, timeArray );
	}
}


