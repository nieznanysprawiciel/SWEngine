/**
@file SpawnPoint.cpp
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/


#include "EngineCore/stdafx.h"
#include "SpawnPoint.h"

#include "swCommonLib/Common/MemoryLeaks.h"


RTTR_REGISTRATION
{
	rttr::registration::class_< sw::SpawnPoint >( "SpawnPoint" )
		.property( "SpawnGroup", &sw::SpawnPoint::m_spawnGroup );
}



namespace sw
{


 /**@brief */
SpawnPoint::SpawnPoint()
{}

/**@brief */
SpawnPoint::~SpawnPoint()
{}

}	// sw

