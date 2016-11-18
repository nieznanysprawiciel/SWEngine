#pragma once
/**
@file Converter.h
@author nieznanysprawiciel
@copyright File is part of graphic engine SWEngine.
*/

#include <vector>

#include "Tools/Geometric/Types/IndexedGeometry.h"



namespace Geometric
{





/**@brief Converts different types of geoemtry.*/
template< typename VertexType, typename IndexType >
class Converter
{
public:
	typedef IndexedGeometry< VertexType, IndexType > ResultGeometry;

private:
protected:
public:
	explicit		Converter() = default;
					~Converter() = default;

	/**@brief Function computes indexed vertex and index buffer. Each vertex is unique in created array.

	Operator < for VertexType must be defined.

	@param[in] srcVerticies Vector of triangles.
	@return Indexed geometry.*/
	static ResultGeometry		MakeIndexed		( const std::vector< VertexType >& srcVerticies );

};


}	// Geometric

#include "Converter.inl"
