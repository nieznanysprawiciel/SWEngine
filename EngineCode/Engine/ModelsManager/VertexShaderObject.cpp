#include "..\..\stdafx.h"
#include "meshes_textures_materials.h"


//----------------------------------------------------------------------------------------------//
//								contructors, destructors										//
//----------------------------------------------------------------------------------------------//

VertexShaderObject::VertexShaderObject()
: referenced_object( WRONG_ID )
{
	vertex_shader = nullptr;
}



VertexShaderObject::~VertexShaderObject( )
{
	if ( vertex_shader )
		vertex_shader->Release( );
}


/*Tworzy obiekt VertexShaderObject na podstawie pliku.
Na razie obs�uguje tylko nieskompilowane pliki.*/
VertexShaderObject* VertexShaderObject::create_from_file( const std::wstring& file_name, const std::string& shader_name )
{
	HRESULT result;
	ID3DBlob* compiled_shader;
	ID3D11VertexShader* vertex_shader;

	// Kompilujemy shader znaleziony w pliku
	D3DX11CompileFromFile( file_name.c_str( ), 0, 0, shader_name.c_str( ), "vs_4_0",
						   0, 0, 0, &compiled_shader, 0, &result );

	if ( FAILED( result ) )
		return nullptr;

	// Tworzymy obiekt shadera na podstawie tego co sie skompilowa�o
	result = device->CreateVertexShader( compiled_shader->GetBufferPointer( ),
										 compiled_shader->GetBufferSize( ),
										 nullptr, &vertex_shader );

	if ( FAILED( result ) )
	{
		compiled_shader->Release( );
		return nullptr;
	}

	// Tworzymy obiekt nadaj�cy si� do u�ycia w silniku i zwracamy wska�nik na niego
	VertexShaderObject* new_shader = new VertexShaderObject( vertex_shader );
	return new_shader;
}
