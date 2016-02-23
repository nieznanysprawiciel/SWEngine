#pragma once
/**@file RenderingApi.h
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.

@brief Deklaracja klasy RenderingApi.*/

class DynamicMeshObject;

#include "EngineCore/MainEngine/UserApi/ApiGroup.h"
#include "EngineCore/MainEngine/UserApi/Actors/Assets.h"

#include <vector>

namespace Api
{

/**@defgroup ActorsAPI Zarz¹dzanie aktorami.
@ingroup UserAPI
@brief Funkcje do zarz¹dzania aktorami.


	/**@brief Zawiera funkcje do obs³ugi aktorów.
	@ingroup UserAPI
	@ingroup ActorsAPI*/
	class ActorsApi : public ApiGroup
	{
	private:
	protected:
	public:
		ActorsApi() = default;
		~ActorsApi() = default;
	public:
		std::vector<DynamicMeshObject*>		GetSceneObjects();	///<@todo Zlikwidowaæ GetSceneObjects. Stworzone tylko dla LightmapsTool.

		void								AddDynamicMesh	( DynamicMeshObject* object );
		void								CleanScene		();
	public:
		Actors::Assets		assets;			///< Pozwala na ustawianie assetów aktorom.
	};


}	// Api
