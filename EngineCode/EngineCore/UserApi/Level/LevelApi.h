#pragma once
/**@file LevelApi.h
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.

@brief Deklaracja klasy InputApi.*/

class InputAbstractionLayer;

#include "EngineCore/UserApi/ApiGroup.h"
#include "EngineCore/UserApi/Level/SerializationApi.h"

#include <string>

namespace Api
{

/**@defgroup LevelApi Zarz¹dzanie levelami.
@ingroup UserAPI
@brief Funkcje do wczytywania levelu.*/

	/**@brief Zawiera funkcje do wczytywania levelu.
	@ingroup UserAPI
	@ingroup LevelApi*/
	class LevelApi : public ApiGroup
	{
	private:
	protected:
	public:
		LevelApi() = default;
		~LevelApi() = default;
	public:

		Api::SerializationApi		Serialization;	///< Funkcje pomocnicze do serializacji.

	public:
	
		bool			LoadLevelSync		( const std::string& levelFileName );
	};



}	// Api
