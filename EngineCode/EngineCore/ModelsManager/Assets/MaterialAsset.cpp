/**
@file MaterialAsset.cpp
@author nieznanysprawiciel
@copyright Plik jest cz�ci� silnika graficznego SWEngine.
*/


#include "EngineCore/stdafx.h"
#include "MaterialAsset.h"

#include "Common/MemoryLeaks.h"


MaterialAsset::MaterialAsset()
	:	ResourceObject( WRONG_ID )
{ }


MaterialAsset::~MaterialAsset()
{ }


//====================================================================================//
//				PhongMaterialData
//====================================================================================//

/**@brief Ustawia materia� na warto�ci domy�lne dla silnika.

Te warto�ci s� najbardziej neutralne, w przypadku gdy nie ma materia�u, a jest ustawiona tekstura.
Wtedy shadery wymana�aj� jasno�� piksela przez 1.0 i nic sie nie zmienia.*/
void PhongMaterialData::SetNullMaterial()
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