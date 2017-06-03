#pragma once
/**@file Materials.h
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.

@brief Deklaracja klasy Materials.*/

struct MaterialObject;

#include "EngineCore/UserApi/ApiGroup.h"
#include "swGraphicAPI/Resources/ResourcePtr.h"

#include <string>


namespace Api { namespace Assets {

	/**@brief Zapewnia dostêp do materia³ów w klasie AssetsManager.
	@ingroup UserAPI
	@ingroup AssetsAPI*/
	class Materials : public ApiGroup
	{
	private:
	protected:
	public:
		Materials() = default;
		~Materials() = default;

		/**@brief Returns material only if it was previously loaded.*/
		ResourcePtr< MaterialAsset >			GetSync					( const std::wstring& name );

		/**@brief Creates completely new material.*/
		ResourcePtr< MaterialAsset >			CreateMaterialSync		( const std::wstring& name, MaterialInitData&& initData );
		/**@brief Creates completely new material.*/
		ResourcePtr< MaterialAsset >			CreateMaterialSync		( const std::wstring& name, MaterialCreateData&& initData );

		std::vector< ResourcePtr< MaterialAsset > >		List	();
	};


}	// Assets
}	// Api
