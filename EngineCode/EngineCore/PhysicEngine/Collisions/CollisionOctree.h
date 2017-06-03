#pragma once
/**@file ColliosionOctree.h
@author nieznanysprawiciel
@copyright Plik jest cz�ci� silnika graficznego SWEngine.

@brief Implementacja drzewa �semkowego kolizji.*/


#include "OctreeLeaf.h"


namespace sw
{


/**@brief */
class CollisionOctree
{
private:

	//std::vector< OctreeLeaf >		m_leafs;
	OctreeNodeType					m_root;

	float							m_dimension;		///< Przestrze� jak� obejmuje drzewo.
	uint16							m_maxDivisions;		///< Liczba podzia��w przestrzeni w drzewie.

protected:
public:
	CollisionOctree();
	~CollisionOctree() = default;

	float			GetDimension	() { return m_dimension; }
	uint16			GetNumDivisions	() { return m_maxDivisions; }

	void			RecreateOctree	( const std::vector< CollisionActor* >& staticMap, float dim, uint16 maxSpaceDivisions );
};

}	// sw
