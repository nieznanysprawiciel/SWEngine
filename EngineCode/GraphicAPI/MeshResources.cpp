#include "ResourcesFactory.h"

//----------------------------------------------------------------------------------------------//
//								PixelShaderObject												//
//----------------------------------------------------------------------------------------------//

PixelShaderObject::PixelShaderObject()
: referenced_object( WRONG_ID )
{
}



/**@bref Tworzy obiekt PixelShaderObject na podstawie pliku.

W przypadku b��d�w kompilacji w trybie debug s� one przekierowane do okna Output.

Na razie obs�uguje tylko nieskompilowane pliki.
@param[in] file_name Nazwa pliku, z kt�rego zostanie wczytany shader.
@param[in] shader_name Nazwa funkcji, kt�ra jest punktem poczatkowym wykonania shadera.
@param[in] shader_model �a�cuch znak�w opisuj�cy shader model.
*/
PixelShaderObject* PixelShaderObject::create_from_file( const std::wstring& file_name, const std::string& shader_name, const char* shader_model )
{
	return ResourcesFactory::CreatePixelShaderFromFile( file_name, shader_name, shader_model );
}

//----------------------------------------------------------------------------------------------//
//								VertexShaderObject												//
//----------------------------------------------------------------------------------------------//

VertexShaderObject::VertexShaderObject()
: referenced_object( WRONG_ID )
{
}



/**@brief Tworzy obiekt VertexShaderObject na podstawie pliku.

W przypadku b��d�w kompilacji w trybie debug s� one przekierowane do okna Output.

Na razie obs�uguje tylko nieskompilowane pliki.
@param[in] file_name Nazwa pliku, z kt�rego zostanie wczytany shader
@param[in] shader_name Nazwa funkcji, kt�ra jest punktem poczatkowym wykonania shadera
@param[in] shader_model �a�cuch znak�w opisuj�cy shader model.
@return Zwraca wska�nik na obiekt VertexShaderObject lub nullptr w przypadku niepowodzenia.*/
VertexShaderObject* VertexShaderObject::create_from_file( const std::wstring& file_name, const std::string& shader_name, const char* shader_model )
{
	return ResourcesFactory::CreateVertexShaderFromFile( file_name, shader_name, shader_model );
}

/**@brief Tworzy obiekt VertexShaderObject na podstawie pliku. Zwraca r�wnie� layout dla podanej struktury wierzcho�ka.
Nie nale�y u�ywa� tej funkcji, je�eli layout nie jest rzeczywi�cie potrzebny. Trzeba pamieta� o zwolnieniu
go, kiedy przestanie by� potrzebny.

W przypadku b��d�w kompilacji w trybie debug s� one przekierowane do okna Output.

Na razie obs�uguje tylko nieskompilowane pliki.
@param[in] file_name Nazwa pliku, z kt�rego zostanie wczytany shader
@param[in] shader_name Nazwa funkcji, kt�ra jest punktem poczatkowym wykonania shadera
@param[out] layout W zmiennej umieszczany jest wska�nik na layout wierzcho�ka. Nale�y pami�ta� o zwolnieniu go kiedy b�dzie niepotrzebny.
@param[in] layout_desc Deskryptor opisujacy tworzony layout.
@param[in] array_size Liczba element�w tablicy layout_desc.
@param[in] shader_model �a�cuch znak�w opisuj�cy shader model.
@return Zwraca wska�nik na obiekt VertexShaderObject lub nullptr w przypadku niepowodzenia.*/
VertexShaderObject* VertexShaderObject::create_from_file( const std::wstring& file_name,
														  const std::string& shader_name,
														  ID3D11InputLayout** layout,
														  D3D11_INPUT_ELEMENT_DESC* layout_desc,
														  unsigned int array_size,
														  const char* shader_model )
{
	HRESULT result;
	ID3DBlob* compiled_shader;
	ID3D11VertexShader* vertex_shader;
	// Troszk� zamieszania, ale w trybie debug warto wiedzie� co jest nie tak przy kompilacji shadera
#ifdef _DEBUG
	ID3D10Blob* error_blob = nullptr;	// Tu znajdzie si� komunikat o b��dzie
#endif

	// Kompilujemy shader znaleziony w pliku
	D3DX11CompileFromFile( file_name.c_str( ), 0, 0, shader_name.c_str( ), shader_model,
						   0, 0, 0, &compiled_shader,
#ifdef _DEBUG
						   &error_blob,	// Funkcja wsadzi informacje o b��dzie w to miejsce
#else
						   0,	// W trybie Release nie chcemy dostawa� b��d�w
#endif
						   &result );

	if ( FAILED( result ) )
	{
#ifdef _DEBUG
		if ( error_blob )
		{
			// B��d zostanie wypisany na standardowe wyj�cie
			OutputDebugStringA( (char*)error_blob->GetBufferPointer( ) );
			error_blob->Release( );
		}
#endif
		layout = nullptr;
		return nullptr;
	}

	// Tworzymy obiekt shadera na podstawie tego co sie skompilowa�o
	result = device->CreateVertexShader( compiled_shader->GetBufferPointer( ),
										 compiled_shader->GetBufferSize( ),
										 nullptr, &vertex_shader );

	if ( FAILED( result ) )
	{
		compiled_shader->Release();
		layout = nullptr;
		return nullptr;
	}

	// Tworzymy layout
	result = device->CreateInputLayout( layout_desc, array_size, compiled_shader->GetBufferPointer( ),
												compiled_shader->GetBufferSize( ), layout );
	compiled_shader->Release( );
	if ( FAILED( result ) )
	{
		layout = nullptr;
		return nullptr;
	}

	// Tworzymy obiekt nadaj�cy si� do u�ycia w silniku i zwracamy wska�nik na niego
	VertexShaderObject* new_shader = new VertexShaderObject( vertex_shader );
	return new_shader;
}



//----------------------------------------------------------------------------------------------//
//								TextureObject													//
//----------------------------------------------------------------------------------------------//

TextureObject::TextureObject()
	: referenced_object( WRONG_ID )
{
}



/**@brief Por�wnuje ze soba dwa obiekty tekstur.

Obiekty s� takie same, kiedy odwo�uj� si� do tego samego pliku.
*/
bool TextureObject::operator==(const TextureObject& object)
{
	if (this->file_path == object.file_path)
		return true;
	return false;
}

/**@brief Por�wnuje ze soba dwa obiekty tekstur.

Obiekty s� takie same, kiedy odwo�uj� si� do tego samego pliku.
*/
bool TextureObject::operator==(const std::wstring& file_name)
{
	if (this->file_path == file_name)
		return true;
	return false;
}

/** @brief Tworzy z podanego pliku obiekt tekstury.

@param[in] file_name Nazwa pliku zawieraj�cego tekstur�
@return Zawraca stworzony wewn�trz obiekt TextureObject z wczytan� tekstur� lub nullptr w przypadku niepowodzenia.
*/
TextureObject* TextureObject::create_from_file( const std::wstring& file_name )
{
	return ResourcesFactory::CreateTextureFromFile( file_name );
}

//----------------------------------------------------------------------------------------------//
//								BufferObject													//
//----------------------------------------------------------------------------------------------//

BufferObject::BufferObject( unsigned int element_size )
: referenced_object( WRONG_ID )
{
}



/**@brief Tworzy bufor wierzcho�k�w lub indeks�w o podanych parametrach.

@param[in] buffer Wska�nik na bufor z danym, kt�re maj� by� przeniesione do bufora DirectXowego.
@param[in] element_size Rozmiar pojedynczego elementu w buforze
@param[in] vert_count Liczba wierzcho�k�w/indeks�w w buforze
@param[in] bind_flag Okre�la z czym zwi�za� podany bufor w potoku graficznym.
Dla bufora wierzcho�k�w nale�y poda� D3D11_BIND_VERTEX_BUFFER, a dla bufora indeks�w
D3D11_BIND_INDEX_BUFFER.
@param[in] usage Jedna z warto�ci D3D11_USAGE, kt�r� nale�y poda� DirectXowi.
@return Wska�nik na BufferObject w przypadku powodzenia lub nullptr, je�eli co� p�jdzie nie tak.*/
BufferObject* BufferObject::create_from_memory( const void* buffer,
												unsigned int element_size,
												unsigned int vert_count,
												unsigned int bind_flag,
												D3D11_USAGE usage )
{
	return ResourcesFactory::CreateBufferFormMemory( buffer, element_size, vert_count, bind_flag, usage );
}


//----------------------------------------------------------------------------------------------//
//								MaterialObject													//
//----------------------------------------------------------------------------------------------//

MaterialObject::MaterialObject( const MaterialObject* material )
: referenced_object(WRONG_ID)
{
	memcpy( this, material, sizeof(MaterialObject) );
}

/**@brief Ustawia materia� na warto�ci domy�lne dla silnika.

Te warto�ci s� najbardziej neutralne, w przypadku gdy nie ma materia�u, a jest ustawiona tekstura.
Wtedy shadery wymana�aj� jasno�� piksela przez 1.0 i nic sie nie zmienia.*/
void MaterialObject::set_null_material( )
{
	Diffuse.x = 1.0f;
	Diffuse.y = 1.0f;
	Diffuse.z = 1.0f;
	Diffuse.w = 1.0f;

	Ambient.x = 1.0f;
	Ambient.y = 1.0f;
	Ambient.z = 1.0f;
	Ambient.w = 1.0f;

	Specular.x = 1.0f;
	Specular.y = 1.0f;
	Specular.z = 1.0f;
	Specular.w = 1.0f;

	Emissive.x = 1.0f;
	Emissive.y = 1.0f;
	Emissive.z = 1.0f;
	Emissive.w = 1.0f;

	Power = 1.0f;
}

#ifndef __UNUSED


//Obiekty s� takie, kiedy struktury materia��w maj� tak� sam� zawarto��
bool MaterialObject::operator==(const MaterialObject& object)
{
	if (*this == object.material)
		return TRUE;
	return FALSE;
}

bool MaterialObject::operator==(const D3DMATERIAL9& object)
{
	if (material.Diffuse.a != object.Diffuse.a
		|| material.Diffuse.b != object.Diffuse.b
		|| material.Diffuse.g != object.Diffuse.g
		|| material.Diffuse.r != object.Diffuse.r)
		return FALSE;
	if (material.Ambient.a != object.Ambient.a
		|| material.Ambient.b != object.Ambient.b
		|| material.Ambient.g != object.Ambient.g
		|| material.Ambient.r != object.Ambient.r)
		return FALSE;
	if (material.Emissive.a != object.Emissive.a
		|| material.Emissive.b != object.Emissive.b
		|| material.Emissive.g != object.Emissive.g
		|| material.Emissive.r != object.Emissive.r)
		return FALSE;
	if (material.Specular.a != object.Specular.a
		|| material.Specular.b != object.Specular.b
		|| material.Specular.g != object.Specular.g
		|| material.Specular.r != object.Specular.r)
		return FALSE;
	if (material.Power != object.Power)
		return FALSE;

	return TRUE;;
}

#endif

