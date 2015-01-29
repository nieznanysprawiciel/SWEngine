#include "..\..\stdafx.h"
#include "meshes_textures_materials.h"


//----------------------------------------------------------------------------------------------//
//								contructors, destructors										//
//----------------------------------------------------------------------------------------------//

PixelShaderObject::PixelShaderObject()
: referenced_object( WRONG_ID )
{
	pixel_shader = nullptr;
}



PixelShaderObject::~PixelShaderObject( )
{
	if ( pixel_shader )
		pixel_shader->Release( );
}

/*Tworzy obiekt PixelShaderObject na podstawie pliku.
Na razie obs³uguje tylko nieskompilowane pliki.*/
PixelShaderObject* PixelShaderObject::create_from_file( const std::wstring& file_name, const std::string& shader_name )
{
	HRESULT result;
	ID3DBlob* compiled_shader;
	ID3D11PixelShader* pixel_shader;

	// Kompilujemy shader znaleziony w pliku
	D3DX11CompileFromFile( file_name.c_str( ), 0, 0, shader_name.c_str( ), "ps_4_0",
						   0, 0, 0, &compiled_shader, 0, &result );

	if ( FAILED( result ) )
		return nullptr;

	// Tworzymy obiekt shadera na podstawie tego co sie skompilowa³o
	result = device->CreatePixelShader( compiled_shader->GetBufferPointer( ),
										 compiled_shader->GetBufferSize( ),
										 nullptr, &pixel_shader );

	if ( FAILED( result ) )
	{
		compiled_shader->Release( );
		return nullptr;
	}

	// Tworzymy obiekt nadaj¹cy siê do u¿ycia w silniku i zwracamy wskaŸnik na niego
	PixelShaderObject* new_shader = new PixelShaderObject( pixel_shader );
	return new_shader;
}
