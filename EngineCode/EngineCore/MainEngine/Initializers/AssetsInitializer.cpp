/**@file AssetsInitializer.cpp
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.

@brief Inicjalizacja domyœlnych assetów.*/

#include "EngineCore/stdafx.h"

#include "EngineCore/MainEngine/Engine.h"
#include "EngineCore/MainEngine/EngineContext.h"
#include "EngineCore/ModelsManager/AssetsManager.h"
#include "EngineCore/DisplayEngine/DisplayEngine.h"
#include "EngineCore/UserApi/Actors/ActorsApi.inl"

#include "GraphicAPI/ResourcesFactory.h"

#include "Common/MemoryLeaks.h"


/**@brief Inicjuje domyœlne assety silnika.*/
bool Engine::InitDefaultAssets()
{
	DefaultAssets::Init();

	ShaderInputLayout* layout;
	Context->modelsManager->LoadVertexShader( DefaultAssets::DEFAULT_VERTEX_SHADER_STRING, DefaultAssets::DEFAULT_VERTEX_SHADER_ENTRY, &layout, DefaultAssets::LAYOUT_POSITION_NORMAL_COORD );
	Context->modelsManager->LoadPixelShader( DefaultAssets::DEFAULT_PIXEL_SHADER_STRING, DefaultAssets::DEFAULT_PIXEL_SHADER_ENTRY );
	Context->modelsManager->LoadPixelShader( DefaultAssets::DEFAULT_TEX_DIFFUSE_PIXEL_SHADER_PATH, DefaultAssets::DEFAULT_PIXEL_SHADER_ENTRY );

	Context->displayEngine->SetLayout( layout );		///@todo Hack. Layout powinien byæ ustawialny dla ka¿dego mesha z osobna. Zlikwidowaæ.

	RenderTargetObject* mainRenderTarget = ResourcesFactory::CreateScreenRenderTarget();
	Context->modelsManager->AddRenderTarget( mainRenderTarget, DefaultAssets::SCREEN_RENDERTARGET_STRING );

	// Renderer state (blending, depth, rasterizer)
	/// @todo In future we should specify explicitly created descriptors. They can change during development process
	/// and engine should be independent.
	Context->modelsManager->CreateBlendingState( DefaultAssets::DEFAULT_BLENDING_STATE_STRING, BlendingInfo() );
	Context->modelsManager->CreateRasterizerState( DefaultAssets::DEFAULT_RASTERIZER_STATE_STRING, RasterizerStateInfo() );
	Context->modelsManager->CreateDepthStencilState( DefaultAssets::DEFAULT_DEPTH_STATE_STRING, DepthStencilInfo() );

	return true;
}



