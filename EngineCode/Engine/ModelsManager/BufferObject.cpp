#include "..\..\stdafx.h"
#include "meshes_textures_materials.h"


//----------------------------------------------------------------------------------------------//
//								contructors, destructors										//
//----------------------------------------------------------------------------------------------//

BufferObject::BufferObject( unsigned int id )
: referenced_object( id )
{
	buffer = nullptr;
}



BufferObject::~BufferObject( )
{
	if ( buffer )
		buffer->Release( );
}