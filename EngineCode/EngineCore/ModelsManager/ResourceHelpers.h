#pragma once

#include "GraphicAPI/MeshResources.h"

#include <cassert>

///**@brief Zwraca referencj� na modyfikowany zas�b.
//
//@param[in] modelPart Fragment mesha, kt�ry zmieniamy
//@param[in] index Indeks zasobu (dla tekstur inaczej nale�y poda� 0)
//@return Zwraca referencj� na zas�b.*/
//template<typename ResourceType>
//inline ResourceType*&			Get( ModelPart& modelPart, uint8 index )
//{	assert( false );	}
//
//template<>
//inline VertexShader*&		Get( ModelPart& modelPart, uint8 index )
//{	return modelPart.vertex_shader;	}
//
//template<>
//inline PixelShader*&		Get( ModelPart& modelPart, uint8 index )
//{	return modelPart.pixel_shader;	}	
//
//template<>
//inline TextureObject*&			Get( ModelPart& modelPart, uint8 index )
//{	return modelPart.texture[ index ];	}
