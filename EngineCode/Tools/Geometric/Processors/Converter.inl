#pragma once
/**
@file Converter.inl
@author nieznanysprawiciel
@copyright File is part of graphic engine SWEngine.
*/
#include "Converter.h"

#include <algorithm>
#include <numeric>



namespace Geometric
{

/**@brief Functor for comparing indicies which reference verticies in specifued vector.

This functor is used by std::sort function.*/
template< typename VertexType, typename IndexType >
class VertexComparator
{
private:
	const std::vector< VertexType >&	m_verticies;
public:
    VertexComparator ( const std::vector< VertexType >& verts ) : m_verticies( verts ) {}


    bool operator() ( int i, int j )
	{
		if( m_verticies[ i ] < m_verticies[ j ] )
			return true;
		return false;
	}
};


// ================================ //
//
template< typename VertexType, typename IndexType >
inline typename Converter< VertexType, IndexType >::ResultGeometry			Converter< VertexType, IndexType >::MakeIndexed	( const std::vector< VertexType >& srcVerticies )
{
	ResultGeometry result;
	result.Indicies.resize( srcVerticies.size() );	// Sincve we have triangle list topology, new index buffer will have the same size as srcVerticies.

	std::vector< IndexType > tmpIndicies;
	tmpIndicies.resize( srcVerticies.size() );

	// Fill temporary index vector with increasing numbers. Note that this vector is index buffer for our source array.
	std::iota( tmpIndicies.begin(), tmpIndicies.end(), 0 );

	VertexComparator< VertexType, IndexType > comparator( srcVerticies );
	std::sort( tmpIndicies.begin(), tmpIndicies.end(), comparator );

	// Iterate through verticies. Equal verticies are placed near each other.
	// Merge these verticies and compute new indiecies.
	IndexType tmpIdx = 0;
	IndexType uniqueVertIdx = 0;
	while( tmpIdx < tmpIndicies.size() )
	{
		// Rewrite srcVerticies( tmpIdx ) to new buffer.
		result.Verticies.push_back( srcVerticies[ tmpIndicies[ tmpIdx ] ] );
		result.Indicies[ tmpIndicies[ tmpIdx ] ] = uniqueVertIdx;


		// Verticies are in increasing order, so this comparision checks if we are still in group of equal verticies.
		while( tmpIdx + 1 < tmpIndicies.size() && !( comparator( tmpIndicies[ tmpIdx ], tmpIndicies[ tmpIdx + 1 ] ) ) )
		{
			result.Indicies[ tmpIndicies[ tmpIdx + 1 ] ] = uniqueVertIdx;
			tmpIdx++;
		}

		uniqueVertIdx++;
		tmpIdx++;
	}

	return result;
}


}

