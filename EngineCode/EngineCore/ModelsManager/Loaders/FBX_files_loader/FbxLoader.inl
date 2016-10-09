#pragma once
/**
@file FbxLoader.inl
@author nieznanysprawiciel
@copyright File is part of graphic engine SWEngine.
*/


#include "FBX_loader.h"
#include <string.h>		// memcpy

// ================================ //
//
template< typename VertexType >
inline void		FBX_loader::CopyVertexBuffer	( const std::vector< VertexType >& verticies, MemoryChunk& destination )
{
	auto dest = destination.GetMemory< VertexType >();
	memcpy( dest, verticies.data(), verticies.size() * sizeof( VertexType ) );
}

// ================================ //
//
template< typename IndexType >
inline void		FBX_loader::CopyIndexBuffer		( const std::vector< std::vector< Index32 > >& indicies, MemoryChunk& destination )
{
	auto dest = destination.GetMemory< IndexType >();

	for( int i = 0; i < indicies.size(); ++i )
	{
		for( int indexIdx = 0; indexIdx < indicies[ i ].size(); indexIdx++ )
		{
			*dest = indicies[ i ][ indexIdx ];
			dest++;
		}
	}
}

