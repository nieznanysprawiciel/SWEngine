#include "EngineCore/stdafx.h"
#include "PhysicalActor.h"

#include "Common/MemoryLeaks.h"



RTTR_REGISTRATION
{
	rttr::registration::class_< PhysicalActor >( "PhysicalActor" );
}


PhysicalActor::PhysicalActor()
{
	mass = 1;
}


