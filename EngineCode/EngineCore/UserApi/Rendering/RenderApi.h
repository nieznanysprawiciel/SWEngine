#pragma once
/**@file RenderingApi.h
@author nieznanysprawiciel
@copyright Plik jest cz�ci� silnika graficznego SWEngine.

@brief Deklaracja klasy RenderingApi.*/

#include "EngineCore/UserApi/ApiGroup.h"

class RenderPass;
class CameraActor;

namespace Api
{

/**@defgroup RenderingAPI Obs�uga modu��w do renderowania.
@ingroup UserAPI
@brief Funkcje do kontrolowania sposobu renderowania.*/


	/**@brief Zawiera funkcje do obs�ugi wej�cia u�ytkownika.
	@ingroup UserAPI
	@ingroup RenderingAPI*/
	class RenderingApi : public ApiGroup
	{
	private:
	protected:
	public:
		RenderingApi() = default;
		~RenderingApi() = default;
	public:
		void				RenderOnce			( RenderPass* );
		CameraActor*		GetCurrentCamera	();
		void				SetCurrentCamera	( CameraActor* camera );
	};


}	// Api
