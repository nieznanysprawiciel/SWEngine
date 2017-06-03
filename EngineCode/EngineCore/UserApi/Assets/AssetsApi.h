#pragma once
/**@file AssetsApi.h
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.

@brief Deklaracja klasy AssetsApi.*/


#include "EngineCore/UserApi/ApiGroup.h"
#include "EngineCore/UserApi/Assets/Meshes.h"
#include "EngineCore/UserApi/Assets/Animations.h"
#include "EngineCore/UserApi/Assets/Shaders.h"
#include "EngineCore/UserApi/Assets/Textures.h"
#include "EngineCore/UserApi/Assets/Buffers.h"
#include "EngineCore/UserApi/Assets/RenderTargets.h"
#include "EngineCore/UserApi/Assets/Materials.h"



namespace Api
{

/**@defgroup AssetsAPI Zarz¹dzanie assetami.
@ingroup UserAPI
@brief Funkcje do obs³ugi assetów.*/

	/**@brief Zawiera funkcje do zarz¹dzania assetami.
	@ingroup UserAPI
	@ingroup AssetsAPI*/
	class AssetsApi : public ApiGroup
	{
	private:
	protected:
	public:
		AssetsApi() = default;
		~AssetsApi() = default;
	public:
		Assets::Meshes			Meshes;
		Assets::Animations		Animations;
		Assets::Shaders			Shaders;
		Assets::Textures		Textures;
		Assets::Buffers			Buffers;
		Assets::RenderTargets	RenderTargets;
		Assets::Materials		Materials;
	};
	
}	// Api