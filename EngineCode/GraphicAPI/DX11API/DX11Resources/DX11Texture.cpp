#include "DX11Texture.h"

#include "Common/MemoryLeaks.h"

DX11Texture::DX11Texture( const std::wstring& path, ID3D11ShaderResourceView* tex )
{
	m_fileName = path;
	m_texture = tex;
}

DX11Texture::~DX11Texture()
{
	if( m_texture )
		m_texture->Release();
	m_texture = nullptr;
}

/**@brief Zapisuje podan¹ teksturê do pliku.

@brief filePath Œcie¿ka do pliku, do którego ma zostaæ zapisana tekstura.
@return Zwraca true, je¿eli zapis siê uda.*/
bool DX11Texture::SaveToFile( const std::string& filePath )
{
	//HRESULT result = D3DX11SaveTextureToFile( device_context, m_texture, )

	return false;
}

/**@brief Tworzy teksturê na podstawie podanego pliku.

@param[in] fileName Nazwa pliku zawieraj¹cego teksturê
@return Zawraca stworzony wewn¹trz obiekt DX11Texture z wczytan¹ tekstur¹ lub nullptr w przypadku niepowodzenia.
*/
DX11Texture* DX11Texture::CreateFromFile( const std::wstring& fileName )
{
	ID3D11ShaderResourceView* textureView;
	HRESULT result = D3DX11CreateShaderResourceViewFromFile( device, fileName.c_str(),
															 NULL, NULL,
															 &textureView, NULL );
	if( FAILED( result ) )
		return nullptr;

	DX11Texture* texture = new DX11Texture( fileName, textureView );

	return texture;
}

DX11Texture* DX11Texture::CreateFromMemory()
{

	return nullptr;
}
