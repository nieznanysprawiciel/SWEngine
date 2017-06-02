#pragma once
/**@file Models.h
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.

@brief Deklaracja klasy Models.*/

class Model3DFromFile;

#include "EngineCore/UserApi/ApiGroup.h"
#include "swGraphicAPI/Resources/ResourcePtr.h"

#include <string>


namespace Api { namespace Assets {

	/**@brief Zapewnia dostêp do modeli w klasie AssetsManager.
	@ingroup UserAPI
	@ingroup AssetsAPI*/
	class Models : public ApiGroup
	{
	private:
	protected:
	public:
		Models() = default;
		~Models() = default;

		ResourcePtr< MeshAsset >		GetSync		( const std::wstring& name );
		ResourcePtr< MeshAsset >		LoadSync	( const std::wstring& name );

		std::vector< ResourcePtr< MeshAsset > >		List	();
	};


}	// Assets
}	// Api
