#include "EngineCore/stdafx.h"
#include "CollisionOctree.h"




namespace sw
{

/**@brief */
CollisionOctree::CollisionOctree()
{
	//RecreateOctree( 16384.0f, 6 );
}

/**@brief Tworzy drzewo o podanych parametrach.*/
void CollisionOctree::RecreateOctree( const std::vector< CollisionActor* >& staticMap, float dim, uint16 maxSpaceDivisions )
{
	m_dimension = dim;
	m_maxDivisions = maxSpaceDivisions;


}

}	// sw

