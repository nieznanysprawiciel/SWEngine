#pragma once
/**
@file TextureLoader.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/



#include "swCommonLib/Common/TypesDefinitions.h"
#include "swCommonLib/Common/MemoryChunk.h"
#include "swCommonLib/System/Path.h"

#include "swGraphicAPI/Resources/MeshResources.h"


class TextureLoader
{
public:

	static		MemoryChunk		LoadTexture		( const filesystem::Path& filePath, TextureInfo& texInfo );
	static		MemoryChunk		GenerateMipMaps	( MemoryChunk& source, TextureInfo& texInfo );
};

