#include "..\..\stdafx.h"
#include "meshes_textures_materials.h"


//----------------------------------------------------------------------------------------------//
//								contructors, destructors										//
//----------------------------------------------------------------------------------------------//

BufferObject::BufferObject( unsigned int element_size )
: referenced_object( WRONG_ID )
{
	buffer = nullptr;
	stride = element_size;
}


BufferObject::BufferObject( unsigned int element_size, ID3D11Buffer* buff )
: referenced_object( WRONG_ID )
{
	buffer = buff;
	stride = element_size;
}



BufferObject::~BufferObject( )
{
	if ( buffer )
		buffer->Release( );
}

BufferObject* BufferObject::create_from_memory( const void* buffer,
												unsigned int element_size,
												unsigned int vert_count,
												unsigned int bind_flag,
												D3D11_USAGE usage )
{
	// Wype³niamy deskryptor bufora
	D3D11_BUFFER_DESC buffer_desc;
	ZeroMemory( &buffer_desc, sizeof(buffer_desc) );
	buffer_desc.Usage = usage;
	buffer_desc.BindFlags = bind_flag;
	buffer_desc.ByteWidth = vert_count*element_size;

	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory( &InitData, sizeof(InitData) );
	InitData.pSysMem = buffer;


	HRESULT result;
	ID3D11Buffer* new_buffer;
	result = device->CreateBuffer( &buffer_desc, &InitData, &new_buffer );
	if ( FAILED( result ) )
		return nullptr;

	BufferObject* new_buffer_object = new BufferObject(  element_size, new_buffer );
	return new_buffer_object;
}
