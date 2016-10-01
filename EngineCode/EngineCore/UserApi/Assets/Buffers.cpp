#include "EngineCore/stdafx.h"
#include "Buffers.h"

#include "EngineCore/MainEngine/EngineContext.h"

namespace Api { namespace Assets
{


/**@brief Tworzy bufor wierzcho³ków.

@param[in] name Nazwa bufora.
@param[in] data MemoryChunk zawieraj¹cy obszar pamiêci do przekopiowania do bufora.
@param[in] vertCount Liczba wierzcho³ków w buforze. Rozmiar pojedynczego wierzcho³ka zostanie wydedukowany z pozosta³ych danyhch.
@return Zwraca bufor wierzcho³ków lub nullptr.*/
ResourcePtr< BufferObject >						Buffers::CreateVertexBufferSync		( const std::wstring& name, MemoryChunk& data, unsigned int vertCount )
{
	return Context->modelsManager->CreateVertexBuffer( name, data.GetMemory<void>(), data.GetMemorySize() / vertCount, vertCount );
}

/**@copydoc AssetsManager::ListVertexBuffers.*/
std::vector< ResourcePtr< BufferObject > >		Buffers::ListVertexBuffers()
{
	return Context->modelsManager->ListVertexBuffers();
}

/**@copydoc AssetsManager::ListIndexBuffers.*/
std::vector< ResourcePtr< BufferObject > >		Buffers::ListIndexBuffers()
{
	return Context->modelsManager->ListIndexBuffers();
}

/**@copydoc AssetsManager::ListConstantBuffers.*/
std::vector< ResourcePtr< BufferObject > >		Buffers::ListConstantBuffers()
{
	return Context->modelsManager->ListConstantBuffers();
}


}	// Assets
}	// Api