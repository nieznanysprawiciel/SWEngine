#pragma once
/**@file Meshes.h
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.

@brief Deklaracja klasy Meshes.*/

class Model3DFromFile;

#include "EngineCore/UserApi/ApiGroup.h"
#include "GraphicAPI/ResourcePtr.h"

#include <string>


namespace Api { namespace Assets {

	/**@brief Zapewnia dostêp do modeli w klasie AssetsManager.
	@ingroup UserAPI
	@ingroup AssetsAPI*/
	class Meshes : public ApiGroup
	{
	private:
	protected:
	public:
		Meshes() = default;
		~Meshes() = default;

		ResourcePtr< MeshAsset >		GetSync			( const std::wstring& name );
		ResourcePtr< MeshAsset >		LoadSync		( const std::wstring& name );

		ResourcePtr< MeshAsset >		CreateMeshSync	( const std::wstring& name, MeshInitData&& initData );
		ResourcePtr< MeshAsset >		CreateMeshSync	( const std::wstring& name, MeshCreateData&& initData );

		std::vector< ResourcePtr< MeshAsset > >		List	();
	};


}	// Assets
}	// Api
