#pragma once

#include "GraphicAPI/MeshResources.h"

#include <cassert>

///**@brief Zwraca referencjê na modyfikowany zasób.
//
//@param[in] modelPart Fragment mesha, który zmieniamy
//@param[in] index Indeks zasobu (dla tekstur inaczej nale¿y podaæ 0)
//@return Zwraca referencjê na zasób.*/
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
