#pragma once
/**@file IOctreeNode.h
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.

@brief Klasa bazowa node'a.*/


#include "swCommonLib/Common/TypesDefinitions.h"



namespace sw
{

enum OctreeNodeType : uint8
{
	LeafNode,
	RegularNode
};

/**@brief */
class IOctreeNode
{
private:
	OctreeNodeType		m_type;

protected:
public:
	IOctreeNode( OctreeNodeType type ) : m_type( type ) {};
	~IOctreeNode() = default;

	OctreeNodeType		GetType() { return m_type; }
};

}	// sw

