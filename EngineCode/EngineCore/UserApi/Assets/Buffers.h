#pragma once
/**@file Buffers.h
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.

@brief Deklaracja klasy Buffers.*/

class BufferObject;

#include "EngineCore/UserApi/ApiGroup.h"
#include "GraphicAPI/ResourcePtr.h"
#include "Common/MemoryChunk.h"

#include <string>

namespace Api { namespace Assets {

	/**@brief Zapewnia dostêp do modeli w klasie ModelsManager.
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

		std::vector< ResourcePtr< BufferObject > >			ListVertexBuffers	();
		std::vector< ResourcePtr< BufferObject > >			ListIndexBuffers	();
		std::vector< ResourcePtr< BufferObject > >			ListConstantBuffers	();
	};


}	// Assets
}	// Api
