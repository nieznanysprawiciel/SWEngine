#pragma once
/**@file RenderingApi.h
@author nieznanysprawiciel
@copyright Plik jest cz�ci� silnika graficznego SWEngine.

@brief Deklaracja klasy RenderingApi.*/

#include "EngineCore/MainEngine/UserApi/ApiGroup.h"

class RenderPass;


namespace Api
{

	/**@brief Zawiera funkcje do obs�ugi wej�cia u�ytkownika.*/
	class RenderingApi : public ApiGroup
	{
	private:
	protected:
	public:
		RenderingApi() = default;
		~RenderingApi() = default;
	public:
		void		RenderOnce		( RenderPass* );
	};


}	// Api
