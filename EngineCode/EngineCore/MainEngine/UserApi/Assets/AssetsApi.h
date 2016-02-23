#pragma once
/**@file AssetsApi.h
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.

@brief Deklaracja klasy AssetsApi.*/


#include "EngineCore/MainEngine/UserApi/ApiGroup.h"
#include "EngineCore/MainEngine/UserApi/Assets/Models.h"
#include "EngineCore/MainEngine/UserApi/Assets/Animations.h"
#include "EngineCore/MainEngine/UserApi/Assets/Shaders.h"
#include "EngineCore/MainEngine/UserApi/Assets/Textures.h"
#include "EngineCore/MainEngine/UserApi/Assets/Buffers.h"
#include "EngineCore/MainEngine/UserApi/Assets/RenderTargets.h"
#include "EngineCore/MainEngine/UserApi/Assets/Materials.h"



namespace Api
{

	/**@brief Zawiera funkcje do zarz¹dzania assetami.*/
	class AssetsApi : public ApiGroup
	{
	private:
	protected:
	public:
		AssetsApi() = default;
		~AssetsApi() = default;
	public:
		Assets::Models			models;
		Assets::Animations		animations;
		Assets::Shaders			shaders;
		Assets::Textures		textures;
		Assets::Buffers			buffers;
		Assets::RenderTargets	renderTargets;
		Assets::Materials		materials;
	};
	
}	// Api