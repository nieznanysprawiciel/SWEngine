#pragma once
/**
@file ResourcesFactory.h
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.
*/

#include "GraphicAPI/MeshResources.h"
#include "GraphicAPI/IGraphicAPIInitializer.h"

class ModelsManager;


/**@brief Klasa ze statycznymi funkcjami do tworzenia obiektów assetów.
@ingroup GraphicAPI

Jest potrzebna, ¿eby wybraæ implementacjê resourców zgodn¹ z typem renderera.
Ka¿de API graficzne powinno zaimplementowaæ tê klasê.
*/
class ResourcesFactory
{
	friend class ModelsManager;
private:
	static TextureObject*			CreateTextureFromFile			( const std::wstring& fileName );
	static VertexShaderObject*		CreateVertexShaderFromFile		( const std::wstring& fileName, const std::string& shaderName, const char* shaderModel = "vs_4_0" );
	static PixelShaderObject*		CreatePixelShaderFromFile		( const std::wstring& fileName, const std::string& shaderName, const char* shaderModel = "ps_4_0" );
	static ComputeShaderObject*		CreateComputeShaderFromFile		( const std::wstring& fileName, const std::string& shaderName, const char* shaderModel = "cs_4_0" );
	static BufferObject*			CreateBufferFromMemory			( const void* buffer,
																	unsigned int elementSize,
																	unsigned int vertCount,
																	ResourceBinding bindFlag,
																	ResourceUsage usage = ResourceUsage::RESOURCE_USAGE_STATIC );

	static VertexShaderObject*		CreateVertexShaderFromFile		( const std::wstring& fileName,
																	const std::string& shaderName,
																	ShaderInputLayoutObject** layout,
																	InputLayoutDescriptor* layout_desc,
																	const char* shaderModel = "vs_4_0" );
public:
	static IGraphicAPIInitializer*	CreateAPIInitializer			();
	static RenderTargetObject*		CreateScreenRenderTarget		();
	static InputLayoutDescriptor*	CreateInputLayoutDescritor		( const std::wstring& layoutName );
	static RenderTargetObject*		CreateRenderTarget				( const std::wstring& name, const RenderTargetDescriptor& renderTargetDescriptor );
};

