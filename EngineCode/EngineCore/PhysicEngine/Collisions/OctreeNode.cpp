/**
@file OctreeNode.cpp
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/

#include "EngineCore/stdafx.h"
#include "OctreeNode.h"

#include "OctreeLeaf.h"


namespace sw
{


// ================================ //
//
OctreeNode::OctreeNode()
	: IOctreeNode( OctreeNodeType::RegularNode )
{
	for( auto& child : m_children )
	{
		child = nullptr;
	}
}

// ================================ //
//
OctreeNode::~OctreeNode()
{
	for( auto& child : m_children )
	{
		if( child )
		{
			if( child->GetType() == OctreeNodeType::RegularNode )
				delete static_cast<OctreeNode*>( child );
			else
				delete static_cast<OctreeLeaf*>( child );
		}
	}
}


}	// sw
