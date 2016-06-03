#include "EngineCore/stdafx.h"
#include "PhysicalActor.h"

#include "Common/MemoryLeaks.h"



RTTR_REGISTRATION
{
	rttr::registration::class_< PhysicalActor >( "PhysicalActor" )
		.property( "Mass", &PhysicalActor::mass )
		(
			rttr::metadata( MetaDataType::Category, "Physical properties" )
		);
}


PhysicalActor::PhysicalActor()
{
	mass = 1;
}


