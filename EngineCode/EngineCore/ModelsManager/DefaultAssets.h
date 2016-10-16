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
struct VertexNormalTexCoord
{
	DirectX::XMFLOAT3	Position;		///<Pozycja wierzcho�ka.
	DirectX::XMFLOAT3	Normal;			///<Wektor normalny wierzcho�ka.
	DirectX::XMFLOAT2	TexCoord;		///<Wsp�rz�dne tekstury.
};


/// @brief Struktura wierzcho�ka stworzona z my�l� o GUI.
struct VertexTexCoord
{
	DirectX::XMFLOAT3	Position;		///<Pozycja wierzcho�ka.
	DirectX::XMFLOAT2	TexCoord;		///<Wsp�rz�dne tekstury.
};

/// @brief Struktura wierzcho�ka dla bezteksturowych obiekt�w.
struct VertexColor
{
	DirectX::XMFLOAT3	Position;		///<Pozycja wierzcho�ka
	DirectX::XMFLOAT3	Color;			///<Kolor wierzcho�ka
};

/// @brief Struktura u�ywana do produkowania lightmap.
struct CoordColor
{
	DirectX::XMFLOAT2	TexCoord;	///<Wspo�rz�dne lightmapy.
	DirectX::XMFLOAT3	Color;		///<Kolor dla danej wsp�rz�dnej.
};

// RenderTargety
const wchar_t SCREEN_RENDERTARGET_STRING[] = L"::Screen render target";
const wchar_t EDITOR_RENDERTARGET_STRING[] = L"::Editor render target";

// Nazwy dla domy�lnych shader�w i materia�u
const wchar_t DEFAULT_MATERIAL_STRING[]				= L"::default_material";						///<Neutralny materia�.
const wchar_t DEFAULT_VERTEX_SHADER_STRING[]		= L"shaders/default_shaders.fx";				///<Shader bez obs�ugi tekstur.
const wchar_t DEFAULT_PIXEL_SHADER_STRING[]			= L"shaders/default_shaders.fx";				///<Shader bez obs�ugi tekstur.
const wchar_t DEFAULT_TEX_DIFFUSE_PIXEL_SHADER_PATH[]	= L"shaders\\tex_diffuse_shader.fx";		///<Shader z obs�ug� tesktury diffuse.
const wchar_t DEFAULT_COORD_COLOR_PIXEL_SHADER_PATH[]	= L"shaders/LightmapGen.fx";				///<Shader do generowania lightmap.
const wchar_t DEFAULT_COORD_COLOR_VERTEX_SHADER_PATH[]	= L"shaders/LightmapGen.fx";				///<Shader do generowania lightmap.
const wchar_t DEFAULT_LIGHTMAP_PIXEL_SHADER_PATH[]		= L"shaders/MaterialLightmap.fx";			///<Shader u�ywaj�cy materia�u i lightmapy.

// Domy�lne nazwy funkcji w vertex i pixel shaderze
const char DEFAULT_VERTEX_SHADER_ENTRY[] = "vertex_shader";		///<Domy�lna nazwa funkcji, od kt�rej zaczyna si� wykonanie vertex shadera
const char DEFAULT_PIXEL_SHADER_ENTRY[] = "pixel_shader";		///<Domy�lna nazwa funkcji, od kt�rej zaczyna si� wykonanie pixel shadera


// Semantyki dla vertex shadera
const char SEMANTIC_POSITION[]		= "POSITION";
const char SEMANTIC_NORMAL[]		= "NORMAL";
const char SEMANTIC_TEXCOORD[]		= "TEXCOORD";
const char SEMANTIC_COLOR[]			= "COLOR";


class DefaultAssets
{
private:
	DefaultAssets();
	~DefaultAssets();
protected:
public:
	static InputLayoutDescriptor*			LAYOUT_POSITION_NORMAL_COORD;
	static InputLayoutDescriptor*			LAYOUT_POSITION_COORD;
	static InputLayoutDescriptor*			LAYOUT_POSITION_COLOR;
	static InputLayoutDescriptor*			LAYOUT_COORD_COLOR;

public:
	static void Init();
	static void Release();
};

