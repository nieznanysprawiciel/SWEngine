#include "GraphicAPI/ResourcesFactory.h"

//----------------------------------------------------------------------------------------------//
//								PixelShaderObject												//
//----------------------------------------------------------------------------------------------//

#ifndef __UNUSED
/**@bref Tworzy obiekt PixelShaderObject na podstawie pliku.

W przypadku b³êdów kompilacji w trybie debug s¹ one przekierowane do okna Output.

Na razie obs³uguje tylko nieskompilowane pliki.
@param[in] fileName Nazwa pliku, z którego zostanie wczytany shader.
@param[in] shaderName Nazwa funkcji, która jest punktem poczatkowym wykonania shadera.
@param[in] shaderModel £añcuch znaków opisuj¹cy shader model.
*/
PixelShaderObject* PixelShaderObject::create_from_file( const std::wstring& fileName, const std::string& shaderName, const char* shaderModel )
{
	return ResourcesFactory::CreatePixelShaderFromFile( fileName, shaderName, shaderModel );
}
#endif

//----------------------------------------------------------------------------------------------//
//								VertexShaderObject												//
//----------------------------------------------------------------------------------------------//

#ifndef __UNUSED
/**@brief Tworzy obiekt VertexShaderObject na podstawie pliku.

W przypadku b³êdów kompilacji w trybie debug s¹ one przekierowane do okna Output.

Na razie obs³uguje tylko nieskompilowane pliki.
@param[in] fileName Nazwa pliku, z którego zostanie wczytany shader
@param[in] shaderName Nazwa funkcji, która jest punktem poczatkowym wykonania shadera
@param[in] shaderModel £añcuch znaków opisuj¹cy shader model.
@return Zwraca wskaŸnik na obiekt VertexShaderObject lub nullptr w przypadku niepowodzenia.*/
VertexShaderObject* VertexShaderObject::create_from_file( const std::wstring& fileName, const std::string& shaderName, const char* shaderModel )
{
	return ResourcesFactory::CreateVertexShaderFromFile( fileName, shaderName, shaderModel );
}

/**@brief Tworzy obiekt VertexShaderObject na podstawie pliku. Zwraca równie¿ layout dla podanej struktury wierzcho³ka.
Nie nale¿y u¿ywaæ tej funkcji, je¿eli layout nie jest rzeczywiœcie potrzebny. Trzeba pamietaæ o zwolnieniu
go, kiedy przestanie byæ potrzebny.

W przypadku b³êdów kompilacji w trybie debug s¹ one przekierowane do okna Output.

Na razie obs³uguje tylko nieskompilowane pliki.
@param[in] fileName Nazwa pliku, z którego zostanie wczytany shader
@param[in] shaderName Nazwa funkcji, która jest punktem poczatkowym wykonania shadera
@param[out] layout W zmiennej umieszczany jest wskaŸnik na layout wierzcho³ka. Nale¿y pamiêtaæ o zwolnieniu go kiedy bêdzie niepotrzebny.
@param[in] layoutDesc Deskryptor opisujacy tworzony layout.
@param[in] shaderModel £añcuch znaków opisuj¹cy shader model.
@return Zwraca wskaŸnik na obiekt VertexShaderObject lub nullptr w przypadku niepowodzenia.*/
VertexShaderObject* VertexShaderObject::create_from_file( const std::wstring& fileName,
														  const std::string& shaderName,
														  ShaderInputLayoutObject** layout,
														  InputLayoutDescriptor* layoutDesc,
														  const char* shaderModel )
{
	return ResourcesFactory::CreateVertexShaderFromFile( fileName, shaderName, layout, layoutDesc, shaderModel );
}

#endif

//----------------------------------------------------------------------------------------------//
//								TextureObject													//
//----------------------------------------------------------------------------------------------//


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
bool TextureObject::operator==(const std::wstring& fileName)
{
	if (this->GetFileName() == fileName)
		return true;
	return false;
}

#ifndef __UNUSED
/** @brief Tworzy z podanego pliku obiekt tekstury.

@param[in] fileName Nazwa pliku zawieraj¹cego teksturê
@return Zawraca stworzony wewn¹trz obiekt TextureObject z wczytan¹ tekstur¹ lub nullptr w przypadku niepowodzenia.
*/
TextureObject* TextureObject::create_from_file( const std::wstring& fileName )
{
	return ResourcesFactory::CreateTextureFromFile( fileName );
}
#endif
//----------------------------------------------------------------------------------------------//
//								RenderTargetObject												//
//----------------------------------------------------------------------------------------------//


RenderTargetObject::RenderTargetObject( TextureObject* colorBuffer, TextureObject* depthStencil )
	: m_colorBuffer( colorBuffer ),
	m_depthStencilBuffer( depthStencil )
{
	if( m_colorBuffer )
		m_colorBuffer->add_file_reference();
	if( m_depthStencilBuffer )
		m_depthStencilBuffer->add_file_reference();
}

/**@brief Destruktor kasuje obiekty tekstury g³êbokoœci i bufora koloru, je¿eli nie s¹ u¿ywane.
Je¿eli istniej¹ odwo³ania do tych obiektów, to nie s¹ one kasowane, a jedynie usuwa siê odwo³ania
pochodz¹ce od RenderTargetObject.*/
RenderTargetObject::~RenderTargetObject()
{
	if( m_colorBuffer )
	{
		m_colorBuffer->delete_file_reference();
		if( m_colorBuffer->can_delete() )
			ObjectDeleter<TextureObject>::delete_object( m_colorBuffer, ObjectDeleterKey<TextureObject>() );		
	}
	if( m_depthStencilBuffer )
	{
		m_depthStencilBuffer->delete_file_reference();
		if( m_depthStencilBuffer->can_delete() )
			ObjectDeleter<TextureObject>::delete_object( m_depthStencilBuffer, ObjectDeleterKey<TextureObject>() );
	}
}

//----------------------------------------------------------------------------------------------//
//								BufferObject													//
//----------------------------------------------------------------------------------------------//

BufferObject::BufferObject( unsigned int elementSize, unsigned int elementCount )
	:	m_elementSize( elementSize ),
		m_elementCount( elementCount )
{
}


#ifndef __UNUSED
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
												ResourceBinding bind_flag,
												ResourceUsage usage )
{
	return ResourcesFactory::CreateBufferFromMemory( buffer, element_size, vert_count, bind_flag, usage );
}

#endif

//----------------------------------------------------------------------------------------------//
//								MaterialObject													//
//----------------------------------------------------------------------------------------------//

MaterialObject::MaterialObject( const MaterialObject* material )
: ResourceObject(WRONG_ID)
{
	memcpy( this, material, sizeof(MaterialObject) );
}

/**@brief Ustawia materia³ na wartoœci domyœlne dla silnika.

Te wartoœci s¹ najbardziej neutralne, w przypadku gdy nie ma materia³u, a jest ustawiona tekstura.
Wtedy shadery wymana¿aj¹ jasnoœæ piksela przez 1.0 i nic sie nie zmienia.*/
void MaterialObject::SetNullMaterial( )
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


