#pragma once
/**@file Animations.h
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.

@brief Deklaracja klasy Animations.*/


#include "EngineCore/MainEngine/UserApi/ApiGroup.h"

namespace Api { namespace Assets {

	/**@brief Zapewnia dostêp do modeli w klasie ModelsManager.
	@ingroup UserAPI
	@ingroup AssetsAPI*/
	class Animations : public ApiGroup
	{
	private:
	protected:
	public:
		Animations() = default;
		~Animations() = default;
	};


}	// Assets
}	// Api
