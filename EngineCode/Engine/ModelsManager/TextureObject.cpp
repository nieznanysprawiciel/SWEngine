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

TextureObject::TextureObject(unsigned int id, const std::wstring& path, LPDIRECT3DTEXTURE9 tex)
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


//Obiekty s¹ takie same, kiedy odwo³uj¹ siê do tego samego pliku.
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
