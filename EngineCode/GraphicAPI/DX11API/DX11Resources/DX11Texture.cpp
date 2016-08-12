#include "DX11Texture.h"

#include "../DX11Initializer/DX11ConstantsMapper.h"

#include "Common/MemoryLeaks.h"



DX11Texture::DX11Texture( const std::wstring& path, ID3D11ShaderResourceView* tex )
{
	m_fileName = path;
	m_textureView = tex;
}

DX11Texture::DX11Texture( ID3D11Texture2D* tex, ID3D11ShaderResourceView* texView )
	:	m_texture( tex )
	,	m_textureView( texView )
{}

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

/**@brief Tworzy teksturê z podanego wskaŸnika.

@return Zawraca stworzony wewn¹trz obiekt DX11Texture z wczytan¹ tekstur¹ lub nullptr w przypadku niepowodzenia.*/
DX11Texture* DX11Texture::CreateFromMemory( const MemoryChunk& texData, TextureInfo&& texInfo )
{
	if( texData.IsNull() )
		return nullptr;

	ID3D11Texture2D* texture = nullptr;
	ID3D11ShaderResourceView* texView = nullptr;
	D3D11_TEXTURE2D_DESC texDesc;

	unsigned int ArraySize = 1;
	if( texInfo.TextureType == TextureType::TEXTURE_TYPE_TEXTURE2D_ARRAY || texInfo.TextureType == TextureType::TEXTURE_TYPE_TEXTURE2D_MULTISAMPLE_ARRAY )
		ArraySize = texInfo.ArraySize;

	texDesc.Width				= texInfo.TextureWidth;
	texDesc.Height				= texInfo.TextureHeight;
	texDesc.MipLevels			= 1;
	texDesc.Usage				= DX11ConstantsMapper::Get( texInfo.Usage );
	texDesc.ArraySize			= ArraySize;
	
	
	assert( texInfo.TextureType != TextureType::TEXTURE_TYPE_TEXTURE2D_MULTISAMPLE && texInfo.TextureType != TextureType::TEXTURE_TYPE_TEXTURE2D_MULTISAMPLE_ARRAY );
	texDesc.SampleDesc.Count	= 1;
	texDesc.SampleDesc.Quality	= 0;
	
	texDesc.MiscFlags = 0;
	if( texInfo.IsCubeMap )
		texDesc.MiscFlags |= D3D11_RESOURCE_MISC_TEXTURECUBE;
	if( texInfo.AllowShareResource )
		texDesc.MiscFlags |= D3D11_RESOURCE_MISC_SHARED ;

	texDesc.CPUAccessFlags = 0;
	if( texInfo.CPURead )
		texDesc.CPUAccessFlags |= D3D11_CPU_ACCESS_READ;
	if( texInfo.CPUWrite )
		texDesc.CPUAccessFlags |= D3D11_CPU_ACCESS_WRITE;


	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texDesc.Format = DX11ConstantsMapper::Get( texInfo.Format );
	//if( texInfo.GenerateMipMaps )
	//{
	//	texDesc.BindFlags |= D3D11_BIND_RENDER_TARGET;	// Potrzebne, ¿eby mozna by³o wygenerowaæ mipmapy.
	//	texDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
	//}


	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = texData.GetMemory< int8 >();
	data.SysMemPitch = texData.GetMemorySize() / texInfo.TextureHeight;

	HRESULT result = device->CreateTexture2D( &texDesc, &data, &texture );
	if( result == S_OK )
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;

		if( texInfo.TextureType == TextureType::TEXTURE_TYPE_TEXTURE2D )
		{
			viewDesc.Format = texDesc.Format;
			viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			viewDesc.Texture2D.MostDetailedMip = 0;
			viewDesc.Texture2D.MipLevels = -1;

			result = device->CreateShaderResourceView( texture, &viewDesc, &texView );
			if( result == S_OK )
				return new DX11Texture( texture, texView );
			else 
				return nullptr;
		}
		
		assert( !"Other texture types are not suported" );
	}

	return nullptr;
}

