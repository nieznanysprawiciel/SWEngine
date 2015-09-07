#pragma once


#include "MeshResources.h"

/**Klasa ze statycznymi funkcjami do tworzenia obiektów assetów.

Jest potrzebna, ¿eby wybraæ inplementacjê zgodn¹ z typem renderera.*/
class ResourcesFactory
{
public:
	static TextureObject*			CreateTextureFromFile			( const std::wstring& fileName );
	static VertexShaderObject*		CreateVertexShaderFromFile		( const std::wstring& fileName, const std::string& shaderName, const char* shaderModel );
	static PixelShaderObject*		CreatePixelShaderFromFile		( const std::wstring& fileName, const std::string& shaderName, const char* shaderModel );
	static BufferObject*			CreateBufferFormMemory			( const void* buffer,
																	unsigned int elementSize,
																	unsigned int vertCount,
																	unsigned int bindFlag,
																	ResourceUsage usage = ResourceUsage::RESOURCE_USAGE_STATIC );

	static VertexShaderObject*		CreateVertexShaderFromFile		( const std::wstring& fileName,
																	const std::string& shaderName,
																	ShaderInputLayout** layout,
																	InputLayoutDescriptor* layout_desc,
																	const char* shaderModel = "vs_4_0" );
};

