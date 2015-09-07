#include "ResourcesFactory.h"

//----------------------------------------------------------------------------------------------//
//								PixelShaderObject												//
//----------------------------------------------------------------------------------------------//

PixelShaderObject::PixelShaderObject()
{
}



/**@bref Tworzy obiekt PixelShaderObject na podstawie pliku.

W przypadku b³êdów kompilacji w trybie debug s¹ one przekierowane do okna Output.

Na razie obs³uguje tylko nieskompilowane pliki.
@param[in] file_name Nazwa pliku, z którego zostanie wczytany shader.
@param[in] shader_name Nazwa funkcji, która jest punktem poczatkowym wykonania shadera.
@param[in] shader_model £añcuch znaków opisuj¹cy shader model.
*/
PixelShaderObject* PixelShaderObject::create_from_file( const std::wstring& file_name, const std::string& shader_name, const char* shader_model )
{
	return ResourcesFactory::CreatePixelShaderFromFile( file_name, shader_name, shader_model );
}

//----------------------------------------------------------------------------------------------//
//								VertexShaderObject												//
//----------------------------------------------------------------------------------------------//

VertexShaderObject::VertexShaderObject()
{
}



/**@brief Tworzy obiekt VertexShaderObject na podstawie pliku.

W przypadku b³êdów kompilacji w trybie debug s¹ one przekierowane do okna Output.

Na razie obs³uguje tylko nieskompilowane pliki.
@param[in] file_name Nazwa pliku, z którego zostanie wczytany shader
@param[in] shader_name Nazwa funkcji, która jest punktem poczatkowym wykonania shadera
@param[in] shader_model £añcuch znaków opisuj¹cy shader model.
@return Zwraca wskaŸnik na obiekt VertexShaderObject lub nullptr w przypadku niepowodzenia.*/
VertexShaderObject* VertexShaderObject::create_from_file( const std::wstring& file_name, const std::string& shader_name, const char* shader_model )
{
	return ResourcesFactory::CreateVertexShaderFromFile( file_name, shader_name, shader_model );
}

/**@brief Tworzy obiekt VertexShaderObject na podstawie pliku. Zwraca równie¿ layout dla podanej struktury wierzcho³ka.
Nie nale¿y u¿ywaæ tej funkcji, je¿eli layout nie jest rzeczywiœcie potrzebny. Trzeba pamietaæ o zwolnieniu
go, kiedy przestanie byæ potrzebny.

W przypadku b³êdów kompilacji w trybie debug s¹ one przekierowane do okna Output.

Na razie obs³uguje tylko nieskompilowane pliki.
@param[in] file_name Nazwa pliku, z którego zostanie wczytany shader
@param[in] shader_name Nazwa funkcji, która jest punktem poczatkowym wykonania shadera
@param[out] layout W zmiennej umieszczany jest wskaŸnik na layout wierzcho³ka. Nale¿y pamiêtaæ o zwolnieniu go kiedy bêdzie niepotrzebny.
@param[in] layout_desc Deskryptor opisujacy tworzony layout.
@param[in] shader_model £añcuch znaków opisuj¹cy shader model.
@return Zwraca wskaŸnik na obiekt VertexShaderObject lub nullptr w przypadku niepowodzenia.*/
VertexShaderObject* VertexShaderObject::create_from_file( const std::wstring& file_name,
														  const std::string& shader_name,
														  ShaderInputLayout** layout,
														  InputLayoutDescriptor* layout_desc,
														  const char* shader_model )
{

}



//----------------------------------------------------------------------------------------------//
//								TextureObject													//
//----------------------------------------------------------------------------------------------//

TextureObject::TextureObject()
{
}



/**@brief Porównuje ze soba dwa obiekty tekstur.

Obiekty s¹ takie same, kiedy odwo³uj¹ siê do tego samego pliku.
*/
bool TextureObject::operator==( TextureObject& object)
{
	if (this->GetFileName() == object.GetFileName())
		return true;
	return false;
}

/**@brief Porównuje ze soba dwa obiekty tekstur.

Obiekty s¹ takie same, kiedy odwo³uj¹ siê do tego samego pliku.
*/
bool TextureObject::operator==(const std::wstring& file_name)
{
	if (this->GetFileName() == file_name)
		return true;
	return false;
}

/** @brief Tworzy z podanego pliku obiekt tekstury.

@param[in] file_name Nazwa pliku zawieraj¹cego teksturê
@return Zawraca stworzony wewn¹trz obiekt TextureObject z wczytan¹ tekstur¹ lub nullptr w przypadku niepowodzenia.
*/
TextureObject* TextureObject::create_from_file( const std::wstring& file_name )
{
	return ResourcesFactory::CreateTextureFromFile( file_name );
}

//----------------------------------------------------------------------------------------------//
//								BufferObject													//
//----------------------------------------------------------------------------------------------//

BufferObject::BufferObject( unsigned int elementSize, unsigned int elementCount )
	:	m_elementSize( elementSize ),
		m_elementCount( elementCount )
{
}



/**@brief Tworzy bufor wierzcho³ków lub indeksów o podanych parametrach.

@param[in] buffer WskaŸnik na bufor z danym, które maj¹ byæ przeniesione do bufora DirectXowego.
@param[in] element_size Rozmiar pojedynczego elementu w buforze
@param[in] vert_count Liczba wierzcho³ków/indeksów w buforze
@param[in] bind_flag Okreœla z czym zwi¹zaæ podany bufor w potoku graficznym.
Dla bufora wierzcho³ków nale¿y podaæ D3D11_BIND_VERTEX_BUFFER, a dla bufora indeksów
D3D11_BIND_INDEX_BUFFER.
@param[in] usage Jedna z wartoœci D3D11_USAGE, któr¹ nale¿y podaæ DirectXowi.
@return WskaŸnik na BufferObject w przypadku powodzenia lub nullptr, je¿eli coœ pójdzie nie tak.*/
BufferObject* BufferObject::create_from_memory( const void* buffer,
												unsigned int element_size,
												unsigned int vert_count,
												unsigned int bind_flag,
												ResourceUsage usage )
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

/**@brief Ustawia materia³ na wartoœci domyœlne dla silnika.

Te wartoœci s¹ najbardziej neutralne, w przypadku gdy nie ma materia³u, a jest ustawiona tekstura.
Wtedy shadery wymana¿aj¹ jasnoœæ piksela przez 1.0 i nic sie nie zmienia.*/
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


//Obiekty s¹ takie, kiedy struktury materia³ów maj¹ tak¹ sam¹ zawartoœæ
bool MaterialObject::operator==(const MaterialObject& object)
{
	if (*this == object.material)
		return true;
	return false;
}

bool MaterialObject::operator==(const D3DMATERIAL9& object)
{
	if (material.Diffuse.a != object.Diffuse.a
		|| material.Diffuse.b != object.Diffuse.b
		|| material.Diffuse.g != object.Diffuse.g
		|| material.Diffuse.r != object.Diffuse.r)
		return false;
	if (material.Ambient.a != object.Ambient.a
		|| material.Ambient.b != object.Ambient.b
		|| material.Ambient.g != object.Ambient.g
		|| material.Ambient.r != object.Ambient.r)
		return false;
	if (material.Emissive.a != object.Emissive.a
		|| material.Emissive.b != object.Emissive.b
		|| material.Emissive.g != object.Emissive.g
		|| material.Emissive.r != object.Emissive.r)
		return false;
	if (material.Specular.a != object.Specular.a
		|| material.Specular.b != object.Specular.b
		|| material.Specular.g != object.Specular.g
		|| material.Specular.r != object.Specular.r)
		return false;
	if (material.Power != object.Power)
		return false;

	return true;
}

#endif


