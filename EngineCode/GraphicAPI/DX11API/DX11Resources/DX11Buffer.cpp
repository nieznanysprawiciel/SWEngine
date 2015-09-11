#include "DX11Buffer.h"
#include "../DX11Initializer/DX11ConstantsMapper.h"

#include "Common\memory_leaks.h"

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
