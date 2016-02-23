#pragma once
/**@file RenderingApi.h
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.

@brief Deklaracja klasy RenderingApi.*/

#include "EngineCore/MainEngine/UserApi/ApiGroup.h"

class RenderPass;


namespace Api
{

	/**@brief Zawiera funkcje do obs³ugi wejœcia u¿ytkownika.*/
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
