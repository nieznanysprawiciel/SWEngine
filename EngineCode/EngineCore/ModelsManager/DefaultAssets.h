#pragma once
/**
@file DefaultAssets.h
@author nieznanysprawiciel
@copyright Plik jest cz�ci� silnika graficznego SWEngine.

@brief Plik zawiera definicje format�w wierzcho�k�w, domy�lne layouty wierzcho�k�w
oraz inne domy�lne warto�ci zasob�w.
*/

#include "GraphicAPI/MeshResources.h"

//-------------------------------------------------------------------------------//
//	definicje wierzcho�k�w

/// @brief Strutkura dla standardowego wierzcho�ka.
typedef struct VertexNormalTexCord1
{
	DirectX::XMFLOAT3	position;		///<Pozycja wierzcho�ka.
	DirectX::XMFLOAT3	normal;			///<Wektor normalny wierzcho�ka.
	DirectX::XMFLOAT2	tex_cords;		///<Wsp�rz�dne tekstury.
} VertexNormalTexCord1;


/// @brief Struktura wierzcho�ka stworzona z my�l� o GUI.
typedef struct VertexTexCord1
{
	DirectX::XMFLOAT3	position;		///<Pozycja wierzcho�ka.
	DirectX::XMFLOAT2	tex_cords;		///<Wsp�rz�dne tekstury.
} VertexTexCord1;




// Nazwy dla domy�lnych shader�w i materia�u
const wchar_t DEFAULT_MATERIAL_STRING[] = L"::default_material";								///<Neutralny materia�.
const wchar_t DEFAULT_VERTEX_SHADER_STRING[] = L"shaders/default_shaders.fx";					///<Shader bez obs�ugi tekstur.
const wchar_t DEFAULT_PIXEL_SHADER_STRING[] = L"shaders/default_shaders.fx";					///<Shader bez obs�ugi tekstur.
const wchar_t DEFAULT_TEX_DIFFUSE_PIXEL_SHADER_PATH[] = L"shaders\\tex_diffuse_shader.fx";		///<Shader z obs�ug� tesktury diffuse.


// Domy�lne nazwy funkcji w vertex i pixel shaderze
const char DEFAULT_VERTEX_SHADER_ENTRY[] = "vertex_shader";		///<Domy�lna nazwa funkcji, od kt�rej zaczyna si� wykonanie vertex shadera
const char DEFAULT_PIXEL_SHADER_ENTRY[] = "pixel_shader";		///<Domy�lna nazwa funkcji, od kt�rej zaczyna si� wykonanie pixel shadera


// Semantyki dla vertex shadera
const char SEMANTIC_POSITION[]		= "POSITION";
const char SEMANTIC_NORMAL[]		= "NORMAL";
const char SEMANTIC_TEXCOORD[]		= "TEXCOORD";


class DefaultAssets
{
private:
	DefaultAssets();
	~DefaultAssets();
protected:
public:
	static InputLayoutDescriptor			LAYOUT_POSITION_NORMAL_COORD;
	static InputLayoutDescriptor			LAYOUT_POSITION_COORD;


public:
	static void Init();
};

