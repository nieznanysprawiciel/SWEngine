#include "EngineCore/stdafx.h"
#include "Buffers.h"

#include "EngineCore/MainEngine/EngineContext.h"

namespace Api { namespace Assets
{


/**@brief Tworzy bufor wierzchołków.

@param[in] name Nazwa bufora.
@param[in] data MemoryChunk zawierający obszar pamięci do przekopiowania do bufora.
@param[in] vertCount Liczba wierzchołków w buforze. Rozmiar pojedynczego wierzchołka zostanie wydedukowany z pozostałych danyhch.
@return Zwraca bufor wierzchołków lub nullptr.*/
BufferObject*		Buffers::CreateVertexBufferSync		( const std::wstring& name, MemoryChunk& data, unsigned int vertCount )
{
	return Context->modelsManager->AddVertexBuffer( name, data.GetMemory<void>(), data.GetMemorySize() / vertCount, vertCount );
}


}	// Assets
}	// Api