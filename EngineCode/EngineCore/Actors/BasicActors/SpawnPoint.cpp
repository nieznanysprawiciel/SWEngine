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
	rttr::registration::class_< SpawnPoint >( "SpawnPoint" )
		.property( "SpawnGroup", &SpawnPoint::m_spawnGroup );
}



/**@brief */
SpawnPoint::SpawnPoint()
{ }

/**@brief */
SpawnPoint::~SpawnPoint()
{ }
