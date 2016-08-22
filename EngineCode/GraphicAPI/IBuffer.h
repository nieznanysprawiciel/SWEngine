#pragma once
/**
@file IBuffer.h
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.
*/

#include "GraphicAPI/ResourceObject.h"
#include "Common/MemoryChunk.h"

class IBuffer	:	public ResourceObject
{
	RTTR_ENABLE( ResourceObject )
private:

protected:
	IBuffer() : ResourceObject( 0 ) {}
	virtual ~IBuffer() = default;
public:
	virtual MemoryChunk			CopyData		() = 0;		///<Kopiuje dane z bufora i umieszcza je w zwracanym MemoryChunku.
};

