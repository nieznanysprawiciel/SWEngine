#pragma once
/**@file RenderingApi.h
@author nieznanysprawiciel
@copyright Plik jest cz�ci� silnika graficznego SWEngine.

@brief Deklaracja klasy RenderingApi.*/

class DynamicMeshActor;
class TextureObject;
class PixelShader;
class VertexShader;

#include "EngineCore/UserApi/ApiGroup.h"

namespace Api { namespace Actors {

	/**@brief Zawiera funkcje do ustawiania asset�w dla aktor�w.
	@ingroup UserAPI
	@ingroup ActorsAPI*/
	class Assets : public ApiGroup
	{
	private:
	protected:
	public:
		Assets() = default;
		~Assets() = default;

		void		ChangeTextures		( DynamicMeshActor* mesh, TextureObject* newTex, uint16 beginPart, uint16 endPart, uint8 texIndex );

		void		ChangePixelShaders	( DynamicMeshActor* mesh, PixelShader* newShader, uint16 beginPart, uint16 endPart );
		void		ChangeVertexShaders	( DynamicMeshActor* mesh, VertexShader* newShader, uint16 beginPart, uint16 endPart );
	};


}	// Actors
}	// Api
