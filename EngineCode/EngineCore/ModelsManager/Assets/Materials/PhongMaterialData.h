#pragma once
/**
@file PhongMaterial.h
@author nieznanysprawiciel
@copyright File is part of graphic engine SWEngine.
*/



#include <DirectXMath.h>



namespace sw
{

/**@brief Layout bufora materia³u w klasie @ref MaterialAsset.

@ingroup Materials*/
struct PhongMaterial
{
	DirectX::XMFLOAT4		Diffuse;		///< Kana³ rozpraszania. Komponent w wektora jest kana³em alfa dla ca³êgo materia³u.
	DirectX::XMFLOAT4		Ambient;		///< Kana³ ambient. Komponent w niezu¿ywany.
	DirectX::XMFLOAT4		Specular;		///< Kana³ odbicia zwierciadlanego. Komponent w niezu¿ywany.
	DirectX::XMFLOAT3		Emissive;		///< Kolor materia³u nieoœwietlonego. Komponent w niezu¿ywany.
	float					Power;			///< Wyk³adnik u¿ywany przy obliczaniu odbicia zwierciadlanego.

	inline void SetNullMaterial();
};


/**@brief Ustawia materia³ na wartoœci domyœlne dla silnika.

Te wartoœci s¹ najbardziej neutralne, w przypadku gdy nie ma materia³u, a jest ustawiona tekstura.
Wtedy shadery wymana¿aj¹ jasnoœæ piksela przez 1.0 i nic sie nie zmienia.*/
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

