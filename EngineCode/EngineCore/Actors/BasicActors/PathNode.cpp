/**
@file PathNode.cpp
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/


#include "EngineCore/stdafx.h"
#include "PathNode.h"

#include "swCommonLib/Common/MemoryLeaks.h"


RTTR_REGISTRATION
{
	rttr::registration::class_< sw::PathNode >( "PathNode" );
}

namespace sw
{


/**@brief */
PathNode::PathNode()
{
	//rttr::type::get< PathNode >().
}

/**@brief */
PathNode::~PathNode()
{}

}	// sw

