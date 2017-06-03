#pragma once
/**
@file PhongMaterial.h
@author nieznanysprawiciel
@copyright File is part of graphic engine SWEngine.
*/



#include <DirectXMath.h>



namespace sw
{

/**@brief Layout bufora materia�u w klasie @ref MaterialAsset.

@ingroup Materials*/
struct PhongMaterial
{
	DirectX::XMFLOAT4		Diffuse;		///< Kana� rozpraszania. Komponent w wektora jest kana�em alfa dla ca��go materia�u.
	DirectX::XMFLOAT4		Ambient;		///< Kana� ambient. Komponent w niezu�ywany.
	DirectX::XMFLOAT4		Specular;		///< Kana� odbicia zwierciadlanego. Komponent w niezu�ywany.
	DirectX::XMFLOAT3		Emissive;		///< Kolor materia�u nieo�wietlonego. Komponent w niezu�ywany.
	float					Power;			///< Wyk�adnik u�ywany przy obliczaniu odbicia zwierciadlanego.

	inline void SetNullMaterial();
};


/**@brief Ustawia materia� na warto�ci domy�lne dla silnika.

Te warto�ci s� najbardziej neutralne, w przypadku gdy nie ma materia�u, a jest ustawiona tekstura.
Wtedy shadery wymana�aj� jasno�� piksela przez 1.0 i nic sie nie zmienia.*/
inline void PhongMaterial::SetNullMaterial()
{
	Diffuse.x = 1.0f;
	Diffuse.y = 1.0f;
	Diffuse.z = 1.0f;
	Diffuse.w = 1.0f;

	Ambient.x = 1.0f;
	Ambient.y = 1.0f;
	Ambient.z = 1.0f;
	Ambient.w = 1.0f;

	Specular.x = 1.0f;
	Specular.y = 1.0f;
	Specular.z = 1.0f;
	Specular.w = 1.0f;

	Emissive.x = 0.0f;
	Emissive.y = 0.0f;
	Emissive.z = 0.0f;

	Power = 1.0f;
}


}	// sw

