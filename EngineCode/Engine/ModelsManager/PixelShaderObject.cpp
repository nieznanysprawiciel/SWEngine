#include "..\..\stdafx.h"
#include "meshes_textures_materials.h"


//----------------------------------------------------------------------------------------------//
//								contructors, destructors										//
//----------------------------------------------------------------------------------------------//

PixelShaderObject::PixelShaderObject( unsigned int id )
: referenced_object( id )
{
	pixel_shader = nullptr;
}



PixelShaderObject::~PixelShaderObject( )
{
	if ( pixel_shader )
		pixel_shader->Release( );
}