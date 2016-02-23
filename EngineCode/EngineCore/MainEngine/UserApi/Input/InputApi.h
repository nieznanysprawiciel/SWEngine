#pragma once
/**@file InputApi.h
@author nieznanysprawiciel
@copyright Plik jest cz�ci� silnika graficznego SWEngine.

@brief Deklaracja klasy InputApi.*/

class InputAbstractionLayer;

#include "EngineCore/MainEngine/UserApi/ApiGroup.h"
#include "EngineCore/UIEngine/StandardAbstractionLayers.h"

namespace Api
{

	/**@brief Zawiera funkcje do obs�ugi wej�cia u�ytkownika.*/
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
