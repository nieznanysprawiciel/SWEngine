#include "..\..\stdafx.h"
#include "meshes_textures_materials.h"




#include "..\..\memory_leaks.h"

//----------------------------------------------------------------------------------------------//
//								contructors, destructors										//
//----------------------------------------------------------------------------------------------//

MaterialObject::MaterialObject( const MaterialObject* material )
: referenced_object(WRONG_ID)
{
	memcpy( this, material, sizeof(MaterialObject) );
}

void MaterialObject::set_null_material( )
{
	Diffuse.x = 1.0f;
	Diffuse.y = 1.0f;
	Diffuse.z = 1.0f;
	Diffuse.w = 1.0f;

	Ambient.x = 1.0f;
	Ambient.y = 1.0f;
	Ambient.z = 1.0f;

	Specular.x = 1.0f;
	Specular.y = 1.0f;
	Specular.z = 1.0f;

	Emissive.x = 1.0f;
	Emissive.y = 1.0f;
	Emissive.z = 1.0f;

	Power = 1.0f;
}

#ifndef __UNUSED
//----------------------------------------------------------------------------------------------//
//								contructors, destructors										//
//----------------------------------------------------------------------------------------------//

MaterialObject::MaterialObject(unsigned int id)
	: referenced_object(id)
{
}

MaterialObject::MaterialObject(unsigned int id, const MaterialObject& object)
	: referenced_object(id)
{
	memcpy(&material, &object.material, sizeof(_D3DMATERIAL9));
}

MaterialObject::MaterialObject(unsigned int id, const D3DMATERIAL9& material2)
	: referenced_object(id)
{
	memcpy(&material, &material2, sizeof(_D3DMATERIAL9));
}


MaterialObject::~MaterialObject()
{

}

//Obiekty s� takie, kiedy struktury materia��w maj� tak� sam� zawarto��
bool MaterialObject::operator==(const MaterialObject& object)
{
	if (*this == object.material)
		return TRUE;
	return FALSE;
}

bool MaterialObject::operator==(const D3DMATERIAL9& object)
{
	if (material.Diffuse.a != object.Diffuse.a
		|| material.Diffuse.b != object.Diffuse.b
		|| material.Diffuse.g != object.Diffuse.g
		|| material.Diffuse.r != object.Diffuse.r)
		return FALSE;
	if (material.Ambient.a != object.Ambient.a
		|| material.Ambient.b != object.Ambient.b
		|| material.Ambient.g != object.Ambient.g
		|| material.Ambient.r != object.Ambient.r)
		return FALSE;
	if (material.Emissive.a != object.Emissive.a
		|| material.Emissive.b != object.Emissive.b
		|| material.Emissive.g != object.Emissive.g
		|| material.Emissive.r != object.Emissive.r)
		return FALSE;
	if (material.Specular.a != object.Specular.a
		|| material.Specular.b != object.Specular.b
		|| material.Specular.g != object.Specular.g
		|| material.Specular.r != object.Specular.r)
		return FALSE;
	if (material.Power != object.Power)
		return FALSE;

	return TRUE;;
}

#endif
