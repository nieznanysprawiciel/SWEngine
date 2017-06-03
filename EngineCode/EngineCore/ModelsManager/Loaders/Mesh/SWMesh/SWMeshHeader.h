#pragma once
/**
@file SWMeshHeader.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/



#include "swCommonLib/Common/TypesDefinitions.h"



namespace sw
{

struct SWMeshHeader
{
	char		HeaderIntro[ 7 ];		///< Characters "SWMESH" identyfying format.
	uint32		FileSize;				///< Size of file including this header.

};

}	// sw
