#pragma once


#include "Common/MemoryChunk.h"
#include "GraphicAPI/MeshResources.h"

#include "MeshAsset.h"

#include <vector>



/**@brief Structure used to initialize class @ref MeshAsset.
@ingroup MeshAsset*/
struct MeshAssetInitData
{
	MemoryChunk							VertexBuffer;		///< Data used to fill vertex buffer.
	MemoryChunk							IndexBuffer;		///< Data used to fill index buffer.
	ResourcePtr< ShaderInputLayout >	VertexLayout;		///< Vertex layout descriptor.
	std::vector< MeshPart >				MeshSegments;		///< Mesh segments.
};


/**@brief Structure initializes @ref MeshAsset with existing buffers.
@ingroup MeshAsset*/
struct MeshAssetInitWithExistingData
{
	ResourcePtr< BufferObject >			VertexBuffer;
	ResourcePtr< BufferObject >			IndexBuffer;
	ResourcePtr< ShaderInputLayout >	VertexLayout;
	std::vector< MeshPart >				MeshSegments;		///< Mesh segments.
};


