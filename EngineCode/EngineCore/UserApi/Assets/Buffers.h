#pragma once
/**@file Buffers.h
@author nieznanysprawiciel
@copyright Plik jest cz�ci� silnika graficznego SWEngine.

@brief Deklaracja klasy Buffers.*/

class BufferObject;

#include "EngineCore/UserApi/ApiGroup.h"
#include "Common/MemoryChunk.h"

#include <string>

namespace Api { namespace Assets {

	/**@brief Zapewnia dost�p do modeli w klasie ModelsManager.
	@ingroup UserAPI
	@ingroup AssetsAPI*/
	class Buffers : public ApiGroup
	{
	private:
	protected:
	public:
		Buffers() = default;
		~Buffers() = default;

		BufferObject*		CreateVertexBufferSync	( const std::wstring& name, MemoryChunk& data, unsigned int vertCount );
	};


}	// Assets
}	// Api
