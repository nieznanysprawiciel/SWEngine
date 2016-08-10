/**@file AssetsInitializer.cpp
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.

@brief Inicjalizacja domyœlnych assetów.*/

#include "EngineCore/stdafx.h"

#include "EngineCore/MainEngine/Engine.h"
#include "EngineCore/MainEngine/EngineContext.h"
#include "EngineCore/ModelsManager/ModelsManager.h"
#include "EngineCore/DisplayEngine/DisplayEngine.h"
#include "EngineCore/UserApi/Actors/ActorsApi.inl"
#include "GraphicAPI/ResourcesFactory.h"



/**@brief Inicjuje domyœlne assety silnika.*/
bool Engine::InitDefaultAssets()
{
	DefaultAssets::Init();

	ShaderInputLayout* layout;
	Context->modelsManager->AddVertexShader( DEFAULT_VERTEX_SHADER_STRING, DEFAULT_VERTEX_SHADER_ENTRY, &layout, DefaultAssets::LAYOUT_POSITION_NORMAL_COORD );
	Context->modelsManager->AddPixelShader( DEFAULT_PIXEL_SHADER_STRING, DEFAULT_PIXEL_SHADER_ENTRY );
	Context->modelsManager->AddPixelShader( DEFAULT_TEX_DIFFUSE_PIXEL_SHADER_PATH, DEFAULT_PIXEL_SHADER_ENTRY );

	Context->displayEngine->SetLayout( layout );		///@todo Hack. Layout powinien byæ ustawialny dla ka¿dego mesha z osobna. Zlikwidowaæ.

	MaterialObject* nullMaterial = new MaterialObject();
	nullMaterial->SetNullMaterial();
	Context->modelsManager->AddMaterial( nullMaterial, DEFAULT_MATERIAL_STRING );

	RenderTargetObject* mainRenderTarget = ResourcesFactory::CreateScreenRenderTarget();
	Context->modelsManager->AddRenderTarget( mainRenderTarget, SCREEN_RENDERTARGET_STRING );

	return true;
}



