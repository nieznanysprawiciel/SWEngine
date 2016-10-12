#pragma once
/**
@file ResourcesFactory.h
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.
*/

#include "GraphicAPI/MeshResources.h"
#include "GraphicAPI/RasterizerState.h"
#include "GraphicAPI/BlendingState.h"
#include "DepthStencilState.h"
#include "GraphicAPI/SwapChain.h"
#include "GraphicAPI/IGraphicAPIInitializer.h"

class AssetsManager;


/**@brief Klasa ze statycznymi funkcjami do tworzenia obiektów assetów.
@ingroup GraphicAPI

Jest potrzebna, ¿eby wybraæ implementacjê resourców zgodn¹ z typem renderera.
Ka¿de API graficzne powinno zaimplementowaæ tê klasê.
*/
class ResourcesFactory
{
	friend class ResourceManager;
	friend class AssetsManager;
private:
	static TextureObject*			CreateTextureFromMemory			( const MemoryChunk& texData, TextureInfo&& texInfo );

	static VertexShader*			CreateVertexShaderFromFile		( const std::wstring& fileName, const std::string& shaderName, const char* shaderModel = "vs_4_0" );
	static PixelShader*				CreatePixelShaderFromFile		( const std::wstring& fileName, const std::string& shaderName, const char* shaderModel = "ps_4_0" );
	static ComputeShader*			CreateComputeShaderFromFile		( const std::wstring& fileName, const std::string& shaderName, const char* shaderModel = "cs_4_0" );

	static BufferObject*			CreateBufferFromMemory			( const std::wstring& name, const uint8* data, const BufferInfo& bufferInfo );

	static VertexShader*			CreateVertexShaderFromFile		( const std::wstring& fileName,
																	const std::string& shaderName,
																	ShaderInputLayout** layout,
																	InputLayoutDescriptor* layout_desc,
																	const char* shaderModel = "vs_4_0" );

	static BlendingState*			CreateBlendingState				( const BlendingInfo& info );
	static RasterizerState*			CreateRasterizerState			( const RasterizerStateInfo& info );
	static DepthStencilState*		CreateDepthStencilState			( const DepthStencilInfo& info );

public:
	static IGraphicAPIInitializer*	CreateAPIInitializer			();
	static RenderTargetObject*		CreateScreenRenderTarget		();
	static SwapChain*				CreateScreenSwapChain			( RenderTargetObject* screenRT );
	static InputLayoutDescriptor*	CreateInputLayoutDescriptor		( const std::wstring& layoutName );
	static RenderTargetObject*		CreateRenderTarget				( const std::wstring& name, const RenderTargetDescriptor& renderTargetDescriptor );
};

