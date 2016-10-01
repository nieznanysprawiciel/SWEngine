#pragma once
/**@file Textures.h
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.

@brief Deklaracja klasy Textures.*/

class TextureObject;

#include "EngineCore/UserApi/ApiGroup.h"
#include "GraphicAPI/ResourcePtr.h"

#include <string>


namespace Api { namespace Assets {


	/**@brief Zapewnia dostêp do tekstur w klasie AssetsManager.
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

		std::vector< ResourcePtr< TextureObject > >			List	();
	};


}	// Assets
}	// Api
