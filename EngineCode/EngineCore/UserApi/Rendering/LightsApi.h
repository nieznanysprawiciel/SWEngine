#pragma once
/**@file Lights.h
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.

@brief Deklaracja klasy RenderingApi.*/

#include "EngineCore/UserApi/ApiGroup.h"

class ActorBase;
class LightBase;

namespace Api
{

	/**@brief Zawiera funkcje do obs³ugi wejœcia u¿ytkownika.
	@ingroup UserAPI
	@ingroup RenderingAPI*/
	class LightsApi : public ApiGroup
	{
	private:
	protected:
	public:
		LightsApi() = default;
		~LightsApi() = default;
	public:

		
		void		AddLight	( LightBase* light );		///< Adds light to module.
		void		RemoveLight	( ActorBase* light );		///< Removes light from module.
	};


}	// Api

