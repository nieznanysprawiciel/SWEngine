#pragma once
/**@file InputApi.h
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.

@brief Deklaracja klasy InputApi.*/

class InputAbstractionLayer;

#include "EngineCore/MainEngine/UserApi/ApiGroup.h"
#include "EngineCore/UIEngine/StandardAbstractionLayers.h"

namespace Api
{

/**@defgroup InputAPI Zarz¹dzanie wejœciem/wyjœciem.
@ingroup UserAPI
@brief Funkcje do obs³ugi wejœcia u¿ytkownika.

	/**@brief Zawiera funkcje do obs³ugi wejœcia u¿ytkownika.
	@ingroup UserAPI
	@ingroup InputAPI*/
	class InputApi : public ApiGroup
	{
	private:
	protected:
	public:
		InputApi() = default;
		~InputApi() = default;
	public:
		InputAbstractionLayer*		GetStandardAbstractionLayer		( STANDARD_ABSTRACTION_LAYER layer );
		void						SetAbstractionLayer				( InputAbstractionLayer* layer );
		void						SetStandardAbstractionLayer		( STANDARD_ABSTRACTION_LAYER layer );
	};



}	// Api
