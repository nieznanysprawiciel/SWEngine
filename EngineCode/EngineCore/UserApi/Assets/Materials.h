#pragma once
/**@file Materials.h
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.

@brief Deklaracja klasy Materials.*/

struct MaterialObject;

#include "EngineCore/UserApi/ApiGroup.h"
#include "GraphicAPI/ResourcePtr.h"

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

		ResourcePtr< MaterialAsset >			GetSync	( const std::wstring& name );

		std::vector< ResourcePtr< MaterialAsset > >		List	();
	};


}	// Assets
}	// Api
