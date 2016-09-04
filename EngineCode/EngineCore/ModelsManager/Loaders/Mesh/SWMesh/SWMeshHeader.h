#pragma once

#include "Common/TypesDefinitions.h"


struct SWMeshHeader
{
	char		HeaderIntro[ 7 ];		///< Characters "SWMESH" identyfying format.
	uint32		FileSize;				///< Size of file including this header.

};

