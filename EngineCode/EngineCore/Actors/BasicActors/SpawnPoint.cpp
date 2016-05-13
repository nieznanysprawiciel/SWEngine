#include "EngineCore/stdafx.h"
#include "SpawnPoint.h"

#include "Common/MemoryLeaks.h"


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
