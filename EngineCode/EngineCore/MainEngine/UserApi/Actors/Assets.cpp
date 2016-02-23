#include "EngineCore/stdafx.h"
#include "Assets.h"

#include "EngineCore/MainEngine/EngineContext.h"
#include "EngineCore/ModelsManager/ResourceHelpers.h"

namespace Api { namespace Actors
{

template<typename Resource>
void ChangeResource( DynamicMeshObject* mesh, Resource* newResource, uint16 beginPart, uint16 endPart, uint8 index )
{
	if( !newResource )
		return;

	auto& parts = mesh->GetModelParts();
	Size size = parts.size();
	int max = endPart > size ? (int)size : endPart;
	for( int i = beginPart; i < max; ++i )
	{
		auto& resource = Get<Resource>( parts[ i ], index );
		if( resource )
			resource->DeleteObjectReference();
		resource = newResource;
		newResource->AddObjectReference();
	}
}

/**@brief Zmienia tekstur� w podanym obiekcie.

Zmieniany jest zestaw tekstur mi�dzy beginPart a endPart (wy��cznie). Pod uwag� s� brane tekstury o indeksie texIndex.
Mo�na wstawi� sta�� @ref TextureType.

@param[in] mesh Obiekt, do kt�rego zostanie wstawiona nowa tekstura.
@param[in] newTex Tekstura do ustawienia.
@param[in] beginPart Indeks pierwszego podmesha, w kt�rym zmieniamy zas�b.
@param[in] endPart Indeks za ostatnim elementem, kt�ry zostanie zmieniony.
@param[in] texIndex Indeks w tablicy tekstur [0,ENGINE_MAX_TEXTURES]. Mo�e to by� jedna ze sta�ych TextureType.*/
void Assets::ChangeTextures( DynamicMeshObject* mesh, TextureObject* newTex, uint16 beginPart, uint16 endPart, uint8 texIndex )
{
	ChangeResource<TextureObject>( mesh, newTex, beginPart, endPart, texIndex );
}

/**@brief Zmienia pixel shader w podanym obiekcie.

Zmieniany jest zestaw shader�w mi�dzy beginPart a endPart (wy��cznie).

@param[in] mesh Obiekt, do kt�rego zostanie wstawiona nowy shader.
@param[in] newShader Shader do ustawienia.
@param[in] beginPart Indeks pierwszego podmesha, w kt�rym zmieniamy zas�b.
@param[in] endPart Indeks za ostatnim elementem, kt�ry zostanie zmieniony.*/
void Assets::ChangePixelShaders( DynamicMeshObject* mesh, PixelShaderObject* newShader, uint16 beginPart, uint16 endPart )
{
	ChangeResource<PixelShaderObject>( mesh, newShader, beginPart, endPart, 0 );
}

/**@brief Zmienia vertex shader w podanym obiekcie.

Zmieniany jest zestaw shader�w mi�dzy beginPart a endPart (wy��cznie).

@param[in] mesh Obiekt, do kt�rego zostanie wstawiona nowy shader.
@param[in] newShader Shader do ustawienia.
@param[in] beginPart Indeks pierwszego podmesha, w kt�rym zmieniamy zas�b.
@param[in] endPart Indeks za ostatnim elementem, kt�ry zostanie zmieniony.*/
void Assets::ChangeVertexShaders( DynamicMeshObject* mesh, VertexShaderObject* newShader, uint16 beginPart, uint16 endPart )
{
	ChangeResource<VertexShaderObject>( mesh, newShader, beginPart, endPart, 0 );
}


}	// Actors
}	// Api
