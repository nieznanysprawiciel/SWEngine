#pragma once
/**@file Materials.h
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.

@brief Deklaracja klasy Materials.*/

struct MaterialObject;

#include "EngineCore/MainEngine/UserApi/ApiGroup.h"

#include <string>


namespace Api { namespace Assets {

	/**@brief Zapewnia dostêp do materia³ów w klasie ModelsManager.
	@ingroup UserAPI
	@ingroup AssetsAPI*/
	class Materials : public ApiGroup
	{
	private:
	protected:
	public:
		Materials() = default;
		~Materials() = default;

		MaterialObject*			GetSync( const std::wstring& name );
	};


}	// Assets
}	// Api
