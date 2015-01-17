#include "..\..\stdafx.h"
#include "meshes_textures_materials.h"


//----------------------------------------------------------------------------------------------//
//								contructors, destructors										//
//----------------------------------------------------------------------------------------------//

VertexShaderObject::VertexShaderObject( unsigned int id )
: referenced_object( id )
{
	vertex_shader = nullptr;
}



VertexShaderObject::~VertexShaderObject( )
{
	if ( vertex_shader )
		vertex_shader->Release( );
}