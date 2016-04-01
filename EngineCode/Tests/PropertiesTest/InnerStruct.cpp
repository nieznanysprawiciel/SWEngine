#include "InnerStruct.h"



RTTR_REGISTRATION
{
	rttr::registration::class_< InnerStruct >( "InnerStruct" )
		.property( "Mass", &InnerStruct::Mass )
		.property( "Moment", &InnerStruct::Moment )
		.property( "Radius", &InnerStruct::Radius );
}


InnerStruct::InnerStruct()
{}
