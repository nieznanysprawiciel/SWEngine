#pragma once
/**
@file MipMapGenerator.h
@author nieznanysprawiciel
@copyright Plik jest cz�ci� silnika graficznego SWEngine.
*/

#include "Common/MemoryChunk.h"

#include "GraphicAPI/MeshResources.h"


/**@brief */
class MipMapGenerator
{
private:
public:
	int			NumChannels;

protected:
public:
	MipMapGenerator();
	~MipMapGenerator() = default;

	MemoryChunk		Generate		( MemoryChunk& source, TextureInfo& texInfo );
	void			Resample		( uint32 srcWidth, uint32 srcHeight, uint32 dstWidth, uint32 dstHeight, uint8* source, uint8* dest, MipMapFilter filter );

private:

	static unsigned int		PowerOfTwo				( unsigned int dimmension );
	static uint32			ComputeBufferSize		( unsigned int width, unsigned int height, int bytesPerPixel );
	static unsigned int		ComputeMipMapsLevels	( unsigned int width, unsigned int height );
	static PtrOffset		ComputeMipLevelOffset	( unsigned int initWidth, unsigned int initHeight, int bytesPerPixel, int level );
};


