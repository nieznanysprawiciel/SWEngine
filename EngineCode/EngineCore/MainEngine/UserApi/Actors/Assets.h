#pragma once
/**@file RenderingApi.h
@author nieznanysprawiciel
@copyright Plik jest cz�ci� silnika graficznego SWEngine.

@brief Deklaracja klasy RenderingApi.*/

class DynamicMeshObject;
class TextureObject;
class PixelShaderObject;
class VertexShaderObject;

#include "EngineCore/MainEngine/UserApi/ApiGroup.h"

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

		void		ChangeTextures		( DynamicMeshObject* mesh, TextureObject* newTex, uint16 beginPart, uint16 endPart, uint8 texIndex );

		void		ChangePixelShaders	( DynamicMeshObject* mesh, PixelShaderObject* newShader, uint16 beginPart, uint16 endPart );
		void		ChangeVertexShaders	( DynamicMeshObject* mesh, VertexShaderObject* newShader, uint16 beginPart, uint16 endPart );
	};


}	// Actors
}	// Api
