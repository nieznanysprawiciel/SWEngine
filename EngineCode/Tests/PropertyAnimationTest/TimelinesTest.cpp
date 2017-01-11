#include "catch.hpp"

#include "Common/ParameterAnimation/Timelines/TimelineBase.h"
#include "Common/ParameterAnimation/Timelines/RelativeTimeline.h"

#include "FakeTimeline.h"



TEST_CASE( "Timelines", "[RelativeTimeline]" )
{
	FakeTimelinePtr global = MakePtr< FakeTimeline >();

	RelativeTimelinePtr timeline = MakePtr< RelativeTimeline >( global );

	SECTION( "WrapModes: Clamp, Clamp" )
	{
		timeline->SetDuration( 3.0 );
		timeline->SetPreWrap( WrapMode::Clamp );
		timeline->SetPostWrap( WrapMode::Clamp );


	}


}

