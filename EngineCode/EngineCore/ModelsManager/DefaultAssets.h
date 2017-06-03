#pragma once
/**
@file DefaultAssets.h
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.

@brief Plik zawiera definicje formatów wierzcho³ków, domyœlne layouty wierzcho³ków
oraz inne domyœlne wartoœci zasobów.
*/

#include "swGraphicAPI/Resources/MeshResources.h"


bool		operator==( const DirectX::XMFLOAT2& vertex1, const DirectX::XMFLOAT2& vertex2 );
bool		operator==( const DirectX::XMFLOAT3& vertex1, const DirectX::XMFLOAT3& vertex2 );
bool		operator<( const DirectX::XMFLOAT2& vertex1, const DirectX::XMFLOAT2& vertex2 );
bool		operator<( const DirectX::XMFLOAT3& vertex1, const DirectX::XMFLOAT3& vertex2 );
bool		operator<( const VertexNormalTexCoord& vertex1, const VertexNormalTexCoord& vertex2 );



//-------------------------------------------------------------------------------//
//	definicje wierzcho³ków


namespace sw
{


/// @brief Strutkura dla standardowego wierzcho³ka.
struct VertexNormalTexCoord
{
	DirectX::XMFLOAT3	Position;		///<Pozycja wierzcho³ka.
	DirectX::XMFLOAT3	Normal;			///<Wektor normalny wierzcho³ka.
	DirectX::XMFLOAT2	TexCoord;		///<Wspó³rzêdne tekstury.
};


/// @brief Struktura wierzcho³ka stworzona z myœl¹ o GUI.
struct VertexTexCoord
{
	DirectX::XMFLOAT3	Position;		///<Pozycja wierzcho³ka.
	DirectX::XMFLOAT2	TexCoord;		///<Wspó³rzêdne tekstury.
};

/// @brief Struktura wierzcho³ka dla bezteksturowych obiektów.
struct VertexColor
{
	DirectX::XMFLOAT3	Position;		///<Pozycja wierzcho³ka
	DirectX::XMFLOAT3	Color;			///<Kolor wierzcho³ka
};

/// @brief Struktura u¿ywana do produkowania lightmap.
struct CoordColor
{
	DirectX::XMFLOAT2	TexCoord;	///<Wspo³rzêdne lightmapy.
	DirectX::XMFLOAT3	Color;		///<Kolor dla danej wspó³rzêdnej.
};




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

	// RenderTargety
	static const wchar_t*	SCREEN_RENDERTARGET_STRING;
	static const wchar_t*	EDITOR_RENDERTARGET_STRING;

	// Renderer state
	static const wchar_t*	DEFAULT_BLENDING_STATE_STRING;
	static const wchar_t*	DEFAULT_RASTERIZER_STATE_STRING;
	static const wchar_t*	DEFAULT_DEPTH_STATE_STRING;

	// Nazwy dla domyœlnych shaderów i materia³u
	static const wchar_t*	DEFAULT_MATERIAL_STRING;						///< Neutralny materia³.
	static const wchar_t*	DEFAULT_VERTEX_SHADER_STRING;					///< Shader bez obs³ugi tekstur.
	static const wchar_t*	DEFAULT_PIXEL_SHADER_STRING;					///< Shader bez obs³ugi tekstur.
	static const wchar_t*	DEFAULT_TEX_DIFFUSE_PIXEL_SHADER_PATH;			///< Shader z obs³ug¹ tesktury diffuse.
	static const wchar_t*	DEFAULT_COORD_COLOR_PIXEL_SHADER_PATH;			///< Shader do generowania lightmap.
	static const wchar_t*	DEFAULT_COORD_COLOR_VERTEX_SHADER_PATH;			///< Shader do generowania lightmap.
	static const wchar_t*	DEFAULT_LIGHTMAP_PIXEL_SHADER_PATH;				///< Shader u¿ywaj¹cy materia³u i lightmapy.

	// Domyœlne nazwy funkcji w vertex i pixel shaderze
	static const char*		DEFAULT_VERTEX_SHADER_ENTRY;					///<Domyœlna nazwa funkcji, od której zaczyna siê wykonanie vertex shadera
	static const char*		DEFAULT_PIXEL_SHADER_ENTRY;						///<Domyœlna nazwa funkcji, od której zaczyna siê wykonanie pixel shadera


	// Semantyki dla vertex shadera
	static const char*		SEMANTIC_POSITION;
	static const char*		SEMANTIC_NORMAL;
	static const char*		SEMANTIC_TEXCOORD;
	static const char*		SEMANTIC_COLOR;

public:
	static void Init();
	static void Release();
};


}	// sw

