#pragma once
/**@file Textures.h
@author nieznanysprawiciel
@copyright Plik jest cz�ci� silnika graficznego SWEngine.

@brief Deklaracja klasy Textures.*/

class TextureObject;

#include "EngineCore/MainEngine/UserApi/ApiGroup.h"

#include <string>


namespace Api { namespace Assets {


	/**@brief Zapewnia dost�p do tekstur w klasie ModelsManager.
	@ingroup UserAPI
	@ingroup AssetsAPI*/
	class Textures : public ApiGroup
	{
	private:
	protected:
	public:
		Textures() = default;
		~Textures() = default;

		TextureObject*		GetTexture	( const std::wstring& name );
	};


}	// Assets
}	// Api
