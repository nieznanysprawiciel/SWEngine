#pragma once
/**@file RenderingApi.h
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.

@brief Deklaracja klasy RenderingApi.*/

class StaticActor;
class TextureObject;
class PixelShader;
class VertexShader;

#include "EngineCore/UserApi/ApiGroup.h"

namespace sw {
namespace Api {
namespace Actors {

/**@brief Zawiera funkcje do ustawiania assetów dla aktorów.
@ingroup UserAPI
@ingroup ActorsAPI*/
class Assets : public ApiGroup
{
private:
protected:
public:
	Assets() = default;
	~Assets() = default;

	//void		ChangeTextures		( StaticActor* mesh, TextureObject* newTex, uint16 beginPart, uint16 endPart, uint8 texIndex );

	//void		ChangePixelShaders	( StaticActor* mesh, PixelShader* newShader, uint16 beginPart, uint16 endPart );
	//void		ChangeVertexShaders	( StaticActor* mesh, VertexShader* newShader, uint16 beginPart, uint16 endPart );
};


}	// Actors
}	// Api
}	// sw
