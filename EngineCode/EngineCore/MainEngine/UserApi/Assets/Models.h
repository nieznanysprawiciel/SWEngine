#pragma once
/**@file Models.h
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.

@brief Deklaracja klasy Models.*/

class Model3DFromFile;

#include "EngineCore/MainEngine/UserApi/ApiGroup.h"

#include <string>


namespace Api { namespace Assets {

	/**@brief Zapewnia dostêp do modeli w klasie ModelsManager.
	@ingroup UserAPI
	@ingroup AssetsAPI*/
	class Models : public ApiGroup
	{
	private:
	protected:
	public:
		Models() = default;
		~Models() = default;

		Model3DFromFile*		GetSync		( const std::wstring& name );
		Model3DFromFile*		LoadSync	( const std::wstring& name );
	};


}	// Assets
}	// Api
