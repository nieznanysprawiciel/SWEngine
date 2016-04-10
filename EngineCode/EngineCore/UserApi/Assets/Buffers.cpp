#include "EngineCore/stdafx.h"
#include "Buffers.h"

#include "EngineCore/MainEngine/EngineContext.h"

namespace Api { namespace Assets
{


/**@brief Tworzy bufor wierzcho�k�w.

@param[in] name Nazwa bufora.
@param[in] data MemoryChunk zawieraj�cy obszar pami�ci do przekopiowania do bufora.
@param[in] vertCount Liczba wierzcho�k�w w buforze. Rozmiar pojedynczego wierzcho�ka zostanie wydedukowany z pozosta�ych danyhch.
@return Zwraca bufor wierzcho�k�w lub nullptr.*/
BufferObject*		Buffers::CreateVertexBufferSync		( const std::wstring& name, MemoryChunk& data, unsigned int vertCount )
{
	return Context->modelsManager->AddVertexBuffer( name, data.GetMemory<void>(), data.GetMemorySize() / vertCount, vertCount );
}


}	// Assets
}	// Api