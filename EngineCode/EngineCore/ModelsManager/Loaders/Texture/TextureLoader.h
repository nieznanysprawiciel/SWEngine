#pragma once

#include "Common/TypesDefinitions.h"
#include "Common/MemoryChunk.h"
#include "Common/System/Path.h"

#include "GraphicAPI/MeshResources.h"


class TextureLoader
{
public:

	static		MemoryChunk		LoadTexture		( const filesystem::Path& filePath, TextureInfo& texInfo );
};

