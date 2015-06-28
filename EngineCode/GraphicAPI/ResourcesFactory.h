#pragma once


#if ENABLE_RENDERER == DX11
	#include "GraphicAPI/DX11/DX11VertexShader.h"
	#include "GraphicAPI/DX11/DX11PixelShader.h"
	#include "GraphicAPI/DX11/DX11Buffer.h"
	#include "GraphicAPI/DX11/DX11Texture.h"

	typedef DX11Texture				Texture;
	typedef DX11VertexShader		VertexShader;
	typedef DX11PixelShader			PixelShader;
	typedef DX11Buffer				Buffer;
#else
	#error Nie zdefiniowano API graficznego. Zdefiniuj makro ENABLE_RENDERER w pliku macros_switches.h.
#endif

/**Klasa ze statycznymi funkcjami do tworzenia obiektów assetów.

Jest potrzebna, ¿eby wybraæ inplementacjê zgodn¹ z typem renderera.*/
class ResourcesFactory
{
public:
	static TextureObject*			CreateTextureFromFile( const std::wstring& fileName );
	static VertexShaderObject*		CreateVertexShaderFromFile( const std::wstring& fileName, const std::string& shaderName, const char* shaderModel );
	static PixelShaderObject*		CreatePixelShaderFromFile( const std::wstring& fileName, const std::string& shaderName, const char* shaderModel );
	static BufferObject*			CreateBufferFormMemory( const void* buffer,
															unsigned int elementSize,
															unsigned int vertCount,
															unsigned int bindFlag,
															D3D11_USAGE usage = D3D11_USAGE_DEFAULT );
};

