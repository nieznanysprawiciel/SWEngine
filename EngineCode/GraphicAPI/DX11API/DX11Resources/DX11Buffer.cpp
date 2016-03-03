#include "DX11Buffer.h"
#include "../DX11Initializer/DX11ConstantsMapper.h"

#include "Common/MemoryLeaks.h"

DX11Buffer::DX11Buffer( unsigned int elementSize, unsigned int elementCount, ID3D11Buffer* buff )
	: BufferObject(elementSize, elementCount), m_buffer( buff )
{
}

DX11Buffer::~DX11Buffer()
{
	if( m_buffer )
		m_buffer->Release();
	m_buffer = nullptr;
}

/**@brief Tworzy bufor wierzcho³ków, indeksów lub sta³ych o podanych parametrach.

@param[in] buffer WskaŸnik na bufor z danym, które maj¹ byæ przeniesione do bufora DirectXowego.
@param[in] elementSize Rozmiar pojedynczego elementu w buforze
@param[in] vertCount Liczba wierzcho³ków/indeksów w buforze
@param[in] bindFlag Okreœla z czym zwi¹zaæ podany bufor w potoku graficznym.
Dla bufora wierzcho³ków nale¿y podaæ D3D11_BIND_VERTEX_BUFFER, a dla bufora indeksów
D3D11_BIND_INDEX_BUFFER.
@param[in] usage Jedna z wartoœci D3D11_USAGE, któr¹ nale¿y podaæ DirectXowi.
@return WskaŸnik na DX11Buffer w przypadku powodzenia lub nullptr, je¿eli coœ pójdzie nie tak.*/
DX11Buffer* DX11Buffer::CreateFromMemory(	const void* buffer,
											unsigned int elementSize,
											unsigned int vertCount,
											ResourceBinding bindFlag,
											ResourceUsage usage )
{
	// Wype³niamy deskryptor bufora
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory( &bufferDesc, sizeof( bufferDesc ) );
	bufferDesc.Usage = DX11ConstantsMapper::Get( usage );
	bufferDesc.BindFlags = DX11ConstantsMapper::Get( bindFlag );
	bufferDesc.ByteWidth = vertCount * elementSize;

	D3D11_SUBRESOURCE_DATA* initDataPtr = nullptr;
	D3D11_SUBRESOURCE_DATA initData;
	if( buffer )
	{
		// Je¿eli bufor nie istnieje to do funkcji tworz¹cej bufor powinniœmy podaæ nullptr.
		ZeroMemory( &initData, sizeof( initData ) );
		initData.pSysMem = buffer;
		initDataPtr = &initData;
	}

	HRESULT result;
	ID3D11Buffer* new_buffer;
	result = device->CreateBuffer( &bufferDesc, initDataPtr, &new_buffer );
	if( FAILED( result ) )
		return nullptr;

	DX11Buffer* new_buffer_object = new DX11Buffer( elementSize, vertCount, new_buffer );
	return new_buffer_object;
}


/**@brief Kopiuje pamiêæ bufora i zwraca w MemoryChunku.

Funkcja zwraca zawartoœæ bufora. Pamiêæ jest kopiowana dwukrotnie.
Najpierw na GPU do tymczasowego bufora, a potem po zmapowaniu na pamiêæ RAM,
odbywa siê kopiowanie do MemoryChunka.

@todo Nie trzeba by wykonywaæ kopiowania na GPU, gdyby bufor by³ stworzony z flag¹
D3D11_USAGE_STAGING lub D3D11_USAGE_DEFAULT. Trzeba sprawdziæ flagi i robiæ kopiowanie tylko, gdy to konieczne.

@attention Funkcja nie nadaje siê do wykonania wielow¹tkowego. U¿ywa DeviceContextu do kopiowania danych
w zwi¹zku z czym wymaga synchronizacji z innymi funkcjami renderuj¹cymi.
*/
MemoryChunk DX11Buffer::CopyData()
{
	// Trzeba stworzyæ nowy bufor
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory( &bufferDesc, sizeof( bufferDesc ) );
	bufferDesc.Usage = D3D11_USAGE::D3D11_USAGE_STAGING;
	//bufferDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE;		// Przy fladze usage::staging nie mo¿na bindowaæ zasobu do potoku graficznego.
	bufferDesc.ByteWidth = m_elementSize * m_elementCount;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

	HRESULT result;
	ID3D11Buffer* newBuffer;
	result = device->CreateBuffer( &bufferDesc, nullptr, &newBuffer );
	if( FAILED( result ) )
		return MemoryChunk();

	// Kopiowanie zawartoœci miêdzy buforami
	device_context->CopyResource( newBuffer, m_buffer );

	D3D11_MAPPED_SUBRESOURCE data;
	result = device_context->Map( newBuffer, 0, D3D11_MAP::D3D11_MAP_READ, 0, &data );
	if( FAILED( result ) )
		return MemoryChunk();

	MemoryChunk memoryChunk;
	memoryChunk.MemoryCopy( (int8*)data.pData, m_elementSize * m_elementCount );

	device_context->Unmap( newBuffer, 0 );
	newBuffer->Release();

	return std::move( memoryChunk );
}
