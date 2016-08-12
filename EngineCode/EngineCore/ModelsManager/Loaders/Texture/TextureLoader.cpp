#include "EngineCore/stdafx.h"
#include "TextureLoader.h"

#include "MipMapGenerator.h"


#define STB_IMAGE_IMPLEMENTATION
#include "SOIL/stb_image.h"


/**@brief Wczytuje teksturê z pliku.

Funkcja uzupe³nia w strukturze @ref TextureInfo pola:
- FilePath
- TextureType
- TextureWidth
- TextureHeight
- Format
*/
MemoryChunk			TextureLoader::LoadTexture( const filesystem::Path& filePath, TextureInfo& texInfo )
{
	texInfo.FilePath = filePath;
	texInfo.TextureType = TextureType::TEXTURE_TYPE_TEXTURE2D;

	int height = 0;
	int width = 0;
	int channels = 0;

	unsigned char* data = stbi_load( filePath.String().c_str(), &width, &height, &channels, 4 );

	texInfo.TextureWidth = width;
	texInfo.TextureHeight = height;

	if( data == nullptr )
		return MemoryChunk();

	//if( channels == 1 )
	//	texInfo.format = ResourceFormat::RESOURCE_FORMAT_R8_UNORM;
	//else if( channels == 2 )
	//	texInfo.format = ResourceFormat::RESOURCE_FORMAT_R8G8_UNORM;
	//else if( channels == 3 )
		texInfo.Format = ResourceFormat::RESOURCE_FORMAT_R8G8B8A8_UNORM;

	uint32 size = width * height;
	size *= 4;//channels;

	unsigned char* copiedData = new unsigned char[ size ];
	memcpy( copiedData, data, size );

	MemoryChunk textureData( std::move( (int8*)copiedData ), size );

	stbi_image_free( data );

	if( texInfo.GenerateMipMaps )
		return GenerateMipMaps( textureData, texInfo );
	else
		return textureData;
}

/**@copydoc MipMapGenerator::Generate*/
MemoryChunk TextureLoader::GenerateMipMaps( MemoryChunk& source, TextureInfo& texInfo )
{
	assert( texInfo.GenerateMipMaps );
	assert( texInfo.MipMapFilter < MipMapFilter::Unknown );

	MipMapGenerator mipmaper;
	mipmaper.NumChannels = 4;

	return mipmaper.Generate( source, texInfo );
}
