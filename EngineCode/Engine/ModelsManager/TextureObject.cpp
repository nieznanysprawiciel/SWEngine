#include "..\..\stdafx.h"
#include "meshes_textures_materials.h"


//----------------------------------------------------------------------------------------------//
//								contructors, destructors										//
//----------------------------------------------------------------------------------------------//

TextureObject::TextureObject(unsigned int id)
	: referenced_object(id)
{
	texture = nullptr;
}


/*
//do wywalenia
TextureObject::TextureObject( unsigned int id, const std::wstring& path, LPDIRECT3DTEXTURE9 tex )
: referenced_object( id )
{
	file_path = path;
	texture = tex;
}
*/

TextureObject::TextureObject( unsigned int id, const std::wstring& path, ID3D11ShaderResourceView* tex )
	: referenced_object(id)
{
	file_path = path;
	texture = tex;
}



TextureObject::~TextureObject()
{
	if (texture != nullptr)
		texture->Release();
}


//Obiekty s� takie same, kiedy odwo�uj� si� do tego samego pliku.
bool TextureObject::operator==(const TextureObject& object)
{
	if (this->file_path == object.file_path)
		return TRUE;
	return FALSE;
}

bool TextureObject::operator==(const std::wstring& file_name)
{
	if (this->file_path == file_name)
		return TRUE;
	return FALSE;
}

/*Tworzy z podanego pliku obiekt tekstury o identyfikatorze z pola id.

Generalnie ta funkcja nadaje si� do u�ywania wielow�tkowego. Obiekt directXa
ID3D11Device jest synchronizowany wewn�trznie i nie trzeba si� o to martwi�.
Z tego wzgl�du mo�na tworzy� wiele r�nych zasob�w jednocze�nie.

W przeciwie�stwie do ID3D11Device, ID3D11DeviceContext nie jest synchronizowany,
wi�c nie mo�na renderowa� z wielu w�tk�w jednocze�nie przy u�yciu jednego obiektu.
*/
TextureObject* TextureObject::create_from_file( const std::wstring& file_name )
{
	ID3D11ShaderResourceView* texture_view;
	HRESULT result = D3DX11CreateShaderResourceViewFromFile( device, file_name.c_str(),
															 NULL, NULL,
															 &texture_view, NULL );
	if ( FAILED( result ) )
		return nullptr;

	TextureObject* texture = new TextureObject( WRONG_ID, file_name, texture_view );

	return texture;
}


