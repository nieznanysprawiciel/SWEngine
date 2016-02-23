#pragma once
/**@file RenderingApi.h
@author nieznanysprawiciel
@copyright Plik jest cz�ci� silnika graficznego SWEngine.

@brief Deklaracja klasy RenderingApi.*/

class DynamicMeshObject;

#include "EngineCore/MainEngine/UserApi/ApiGroup.h"
#include "EngineCore/MainEngine/UserApi/Actors/Assets.h"

#include <vector>

namespace Api
{

	/**@brief Zawiera funkcje do obs�ugi wej�cia u�ytkownika.*/
	class ActorsApi : public ApiGroup
	{
	private:
	protected:
	public:
		ActorsApi() = default;
		~ActorsApi() = default;
	public:
		std::vector<DynamicMeshObject*>		GetSceneObjects();	///<@todo Zlikwidowa� GetSceneObjects. Stworzone tylko dla LightmapsTool.

		void								AddDynamicMesh	( DynamicMeshObject* object );
		void								CleanScene		();
	public:
		Actors::Assets		assets;			///< Pozwala na ustawianie asset�w aktorom.
	};


}	// Api