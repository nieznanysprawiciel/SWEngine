#pragma once
/**
@file DefaultAssets.h
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.

@brief Plik zawiera definicje formatów wierzcho³ków, domyœlne layouty wierzcho³ków
oraz inne domyœlne wartoœci zasobów.
*/

#include "GraphicAPI/MeshResources.h"

//-------------------------------------------------------------------------------//
//	definicje wierzcho³ków

/// @brief Strutkura dla standardowego wierzcho³ka.
typedef struct VertexNormalTexCord1
{
	DirectX::XMFLOAT3	position;		///<Pozycja wierzcho³ka.
	DirectX::XMFLOAT3	normal;			///<Wektor normalny wierzcho³ka.
	DirectX::XMFLOAT2	tex_cords;		///<Wspó³rzêdne tekstury.
} VertexNormalTexCord1;


/// @brief Struktura wierzcho³ka stworzona z myœl¹ o GUI.
typedef struct VertexTexCord1
{
	DirectX::XMFLOAT3	position;		///<Pozycja wierzcho³ka.
	DirectX::XMFLOAT2	tex_cords;		///<Wspó³rzêdne tekstury.
} VertexTexCord1;

/// @brief Struktura wierzcho³ka dla bezteksturowych obiektów.
typedef struct VertexColor
{
	DirectX::XMFLOAT3 position;		///<Pozycja wierzcho³ka
	DirectX::XMFLOAT3 color;		///<Kolor wierzcho³ka
} VertexColor;

/// @brief Struktura u¿ywana do produkowania lightmap.
typedef struct CoordColor
{
	DirectX::XMFLOAT2 texCoords;	///<Wspo³rzêdne lightmapy.
	DirectX::XMFLOAT3 color;		///<Kolor dla danej wspó³rzêdnej.
} CoordColor;

// RenderTargety
const wchar_t SCREEN_RENDERTARGET_STRING[] = L"::Screen render target";
const wchar_t EDITOR_RENDERTARGET_STRING[] = L"::Editor render target";

// Nazwy dla domyœlnych shaderów i materia³u
const wchar_t DEFAULT_MATERIAL_STRING[]				= L"::default_material";						///<Neutralny materia³.
const wchar_t DEFAULT_VERTEX_SHADER_STRING[]		= L"shaders/default_shaders.fx";				///<Shader bez obs³ugi tekstur.
const wchar_t DEFAULT_PIXEL_SHADER_STRING[]			= L"shaders/default_shaders.fx";				///<Shader bez obs³ugi tekstur.
const wchar_t DEFAULT_TEX_DIFFUSE_PIXEL_SHADER_PATH[]	= L"shaders\\tex_diffuse_shader.fx";		///<Shader z obs³ug¹ tesktury diffuse.
const wchar_t DEFAULT_COORD_COLOR_PIXEL_SHADER_PATH[]	= L"shaders/LightmapGen.fx";				///<Shader do generowania lightmap.
const wchar_t DEFAULT_COORD_COLOR_VERTEX_SHADER_PATH[]	= L"shaders/LightmapGen.fx";				///<Shader do generowania lightmap.
const wchar_t DEFAULT_LIGHTMAP_PIXEL_SHADER_PATH[]		= L"shaders/MaterialLightmap.fx";			///<Shader u¿ywaj¹cy materia³u i lightmapy.

// Domyœlne nazwy funkcji w vertex i pixel shaderze
const char DEFAULT_VERTEX_SHADER_ENTRY[] = "vertex_shader";		///<Domyœlna nazwa funkcji, od której zaczyna siê wykonanie vertex shadera
const char DEFAULT_PIXEL_SHADER_ENTRY[] = "pixel_shader";		///<Domyœlna nazwa funkcji, od której zaczyna siê wykonanie pixel shadera


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

