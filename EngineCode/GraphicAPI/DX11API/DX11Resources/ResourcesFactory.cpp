#include "GraphicAPI/ResourcesFactory.h"
#include "../DX11Initializer/DX11Initializer.h"

#include "DX11VertexShader.h"
#include "DX11PixelShader.h"
#include "DX11ComputeShader.h"
#include "DX11Buffer.h"
#include "DX11Texture.h"
#include "DX11RenderTarget.h"
#include "DX11InputLayoutDescriptor.h"
#include "DX11InputLayout.h"
#include "DX11SwapChain.h"

#include "Common/MemoryLeaks.h"

typedef DX11Texture				Texture;
typedef DX11VertexShader		VertexShaderObject;
typedef DX11PixelShader			PixelShaderObject;
typedef DX11Buffer				Buffer;
typedef DX11ComputeShader		ComputeShaderObject;


/**@brief Tworzy teksturê z podanego pliku.
@param[in] fileName Nazwa pliku.
@return Zwraca wskaŸnik na obiekt tekstury lub nullptr w przypadku niepowodzenia.*/
TextureObject*			ResourcesFactory::CreateTextureFromFile( const std::wstring& fileName )
{
	return Texture::CreateFromFile( fileName );
}

/**@brief Tworzy obiekt vertex shadera.

@param[in] fileName Nazwa pliku z kodem shadera.
@param[in] shaderName Nazwa funkcji, od której ma siê rozpocz¹æ wykonanie programu shadera.
@param[in] shaderModel Shader model.
@return Zwraca obiekt vertex shadera lub nullptr w przypadku niepowodzenia.*/
VertexShader*		ResourcesFactory::CreateVertexShaderFromFile( const std::wstring& fileName, const std::string& shaderName, const char* shaderModel )
{
	auto shader = VertexShaderObject::CreateFromFile( fileName, shaderName, shaderModel );
	if( shader )
	{
		shader->SetFileName( fileName );
		shader->SetShaderName( shaderName );
	}
	return shader;
}

/**@brief Tworzy obiekt vertex shadera oraz layout wierzcho³ka.

@param[in] fileName Nazwa pliku z kodem shadera.
@param[in] shaderName Nazwa funkcji, od której ma siê rozpocz¹æ wykonanie programu shadera.
@param[out] layout Zwraca obiekt layoutu.
@param[in] layoutDesc Deskryptor opisuj¹cy layout.
@param[in] shaderModel Shader model.
@return Zwraca obiekt vertex shadera lub nullptr w przypadku niepowodzenia.*/
VertexShader*		ResourcesFactory::CreateVertexShaderFromFile	( const std::wstring& fileName,
																		const std::string& shaderName,
																		ShaderInputLayout** layout,
																		InputLayoutDescriptor* layoutDesc,
																		const char* shaderModel/* = "vs_4_0"*/ )
{
	auto shader = VertexShaderObject::CreateFromFile( fileName, shaderName, layout, layoutDesc, shaderModel );
	if( shader )
	{
		shader->SetFileName( fileName );
		shader->SetShaderName( shaderName );
	}
	return shader;
}


/**@brief Tworzy obekt pixel shadera.
@param[in] fileName Nazwa pliku z kodem shadera.
@param[in] shaderName Nazwa funkcji, od której ma siê rozpocz¹æ wykonanie programu shadera.
@param[in] shaderModel Shader model.
@return Zwraca obiekt vertex shadera lub nullptr w przypadku niepowodzenia.*/
PixelShader*		ResourcesFactory::CreatePixelShaderFromFile( const std::wstring& fileName, const std::string& shaderName, const char* shaderModel )
{
	auto shader = PixelShaderObject::CreateFromFile( fileName, shaderName, shaderModel );
	if( shader )
	{
		shader->SetFileName( fileName );
		shader->SetShaderName( shaderName );
	}
	return shader;
}

/**@brief Tworzy obekt compute shadera.
@param[in] fileName Nazwa pliku z kodem shadera.
@param[in] shaderName Nazwa funkcji, od której ma siê rozpocz¹æ wykonanie programu shadera.
@param[in] shaderModel Shader model.
@return Zwraca obiekt vertex shadera lub nullptr w przypadku niepowodzenia.*/
ComputeShader*		ResourcesFactory::CreateComputeShaderFromFile		( const std::wstring& fileName, const std::string& shaderName, const char* shaderModel )
{
	return ComputeShaderObject::CreateFromFile( fileName, shaderName, shaderModel );
}

/**@brief Tworzy bufor na podstawie sanych w pamiêci.

@param[in] buffer WskaŸnik na bufor z danymi.
@param[in] elementSize Rozmiar pojedynczego elementu w buforze.
@param[in] vertCount Liczba wierzcho³ków w buforze.
@param[in] bindFlag Sposób wykorzystania bufora (bufor wierzcho³ków, sta³ych, indeksów itp).
@param[in] usage Sposób korzystania z bufora.
@return Zwraca wskaŸnik na obiekt bufora lub nullptr w przypadku niepowodzenia.*/
BufferObject*			ResourcesFactory::CreateBufferFromMemory( const void* buffer,
																  unsigned int elementSize,
																  unsigned int vertCount,
																  ResourceBinding binding,
																  ResourceUsage usage/* = ResourceUsage::RESOURCE_USAGE_STATIC*/ )
{
	return Buffer::CreateFromMemory( buffer, elementSize, vertCount, binding, usage );
}

/**Tworzy object RenderTargetObject z bufora tylnego ekranu.

@return Zwraca object RenderTargetObject.*/
RenderTargetObject*		ResourcesFactory::CreateScreenRenderTarget()
{
	return DX11RenderTarget::CreateScreenRenderTarget();
}

SwapChain*				ResourcesFactory::CreateScreenSwapChain( RenderTargetObject* screenRT )
{
	return DX11SwapChain::CreateScreenSwapChain( screenRT );
}

IGraphicAPIInitializer* ResourcesFactory::CreateAPIInitializer()
{
	return new DX11Initializer();
}

/**Tworzy obiekt deskryptora layoutu.*/
InputLayoutDescriptor* ResourcesFactory::CreateInputLayoutDescritor( const std::wstring& layoutName )
{
	return new DX11InputLayoutDescriptor( layoutName );
}

RenderTargetObject* ResourcesFactory::CreateRenderTarget( const std::wstring& name, const RenderTargetDescriptor& renderTargetDescriptor )
{
	return DX11RenderTarget::CreateRenderTarget( name, renderTargetDescriptor );
}

