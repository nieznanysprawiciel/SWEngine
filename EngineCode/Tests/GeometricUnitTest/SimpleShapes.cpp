#include "catch.hpp"

#include "Tools/Geometric/Processors/Converter.h"


struct VertexStruct
{
	float x;
	float y;
	float z;

	VertexStruct( float x, float y, float z )
		:	x( x ), y( y ), z( z )	{}

	bool	operator< ( const VertexStruct& vert2 ) const
	{
		if( this->x < vert2.x )
			return true;
		if( this->x > vert2.x )
			return false;

		if( this->y < vert2.y )
			return true;
		if( this->y > vert2.y )
			return false;

		if( this->z < vert2.z )
			return true;
		return false;
	}

	bool	operator==	( const VertexStruct& vert2 ) const
	{
		if( this->x != vert2.x )
			return false;
		if( this->y != vert2.y )
			return false;
		if( this->z != vert2.z )
			return false;
		return true;
	}
};

typedef uint32_t IndexType;


TEST_CASE( "Indexing geometry", "[Geometric]" )
{

	SECTION( "Cube (vertices only)" )
	{
		std::vector< VertexStruct > verticies =
		{
			VertexStruct( -1.0f,-1.0f,-1.0f ),
			VertexStruct( -1.0f,-1.0f, 1.0f ),
			VertexStruct( -1.0f, 1.0f, 1.0f ),

			VertexStruct( 1.0f, 1.0f,-1.0f ),
			VertexStruct( -1.0f,-1.0f,-1.0f ),
			VertexStruct( -1.0f, 1.0f,-1.0f ),

			//
			VertexStruct( 1.0f,-1.0f, 1.0f ),
			VertexStruct( -1.0f,-1.0f,-1.0f ),
			VertexStruct( 1.0f,-1.0f,-1.0f ),

			VertexStruct( 1.0f, 1.0f,-1.0f ),
			VertexStruct( 1.0f,-1.0f,-1.0f ),
			VertexStruct( -1.0f,-1.0f,-1.0f ),

			//
			VertexStruct( -1.0f,-1.0f,-1.0f ),
			VertexStruct( -1.0f, 1.0f, 1.0f ),
			VertexStruct( -1.0f, 1.0f,-1.0f ),

			VertexStruct( 1.0f,-1.0f, 1.0f ),
			VertexStruct( -1.0f,-1.0f, 1.0f ),
			VertexStruct( -1.0f,-1.0f,-1.0f ),

			//
			VertexStruct( -1.0f, 1.0f, 1.0f ),
			VertexStruct( -1.0f,-1.0f, 1.0f ),
			VertexStruct( 1.0f,-1.0f, 1.0f ),

			VertexStruct( 1.0f, 1.0f, 1.0f ),
			VertexStruct( 1.0f,-1.0f,-1.0f ),
			VertexStruct( 1.0f, 1.0f,-1.0f ),

			//
			VertexStruct( 1.0f,-1.0f,-1.0f ),
			VertexStruct( 1.0f, 1.0f, 1.0f ),
			VertexStruct( 1.0f,-1.0f, 1.0f ),

			VertexStruct( 1.0f, 1.0f, 1.0f ),
			VertexStruct( 1.0f, 1.0f,-1.0f ),
			VertexStruct( -1.0f, 1.0f,-1.0f ),

			//
			VertexStruct( 1.0f, 1.0f, 1.0f ),
			VertexStruct( -1.0f, 1.0f,-1.0f ),
			VertexStruct( -1.0f, 1.0f, 1.0f ),

			VertexStruct( 1.0f, 1.0f, 1.0f ),
			VertexStruct( -1.0f, 1.0f, 1.0f ),
			VertexStruct( 1.0f,-1.0f, 1.0f )
		};

		auto result = Geometric::Converter< VertexStruct, IndexType >::MakeIndexed( verticies );

		CHECK( result.Indicies.size() == 36 );
		CHECK( result.Verticies.size() == 8 );

		auto numVertes = result.Verticies.size();

		std::unique( result.Verticies.begin(), result.Verticies.end() );
		CHECK( result.Verticies.size() == numVertes );

	}
}
