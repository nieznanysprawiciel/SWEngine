#pragma once
/**@file Buffers.h
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.

@brief Deklaracja klasy Buffers.*/

class BufferObject;

#include "EngineCore/UserApi/ApiGroup.h"
#include "swGraphicAPI/Resources/ResourcePtr.h"
#include "swCommonLib/Common/MemoryChunk.h"

#include <string>

namespace sw {
namespace Api {
namespace Assets {

/**@brief Zapewnia dostêp do modeli w klasie AssetsManager.
@ingroup UserAPI
@ingroup AssetsAPI*/
class Buffers : public ApiGroup
{
private:
protected:
public:
	Buffers() = default;
	~Buffers() = default;

	ResourcePtr< BufferObject >							CreateVertexBufferSync	( const std::wstring& name, MemoryChunk& data, unsigned int vertCount );
	ResourcePtr< BufferObject >							CreateVertexBufferSync	( const std::wstring& name, uint8* data, uint32 vertCount, uint32 vertexSize );

	ResourcePtr< BufferObject >							CreateIndexBufferSync	( const std::wstring& name, MemoryChunk& data, unsigned int vertCount );
	ResourcePtr< BufferObject >							CreateIndexBufferSync	( const std::wstring& name, uint8* data, uint32 vertCount, uint32 vertexSize );

	std::vector< ResourcePtr< BufferObject > >			ListVertexBuffers	();
	std::vector< ResourcePtr< BufferObject > >			ListIndexBuffers	();
	std::vector< ResourcePtr< BufferObject > >			ListConstantBuffers	();
};


}	// Assets
}	// Api
}	// sw
