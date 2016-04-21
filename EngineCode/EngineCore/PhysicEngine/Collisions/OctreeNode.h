#pragma once
/**@file OctreeNode.h
@author nieznanysprawiciel
@copyright Plik jest cz�ci� silnika graficznego SWEngine.

@brief */

#include "IOctreeNode.h"




class OctreeNode : public IOctreeNode
{
private:

	IOctreeNode*		m_children[ 8 ];

public:
	OctreeNode();
	~OctreeNode();
};

