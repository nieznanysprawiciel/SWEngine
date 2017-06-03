#pragma once
/**
@file DefaultAssets.h
@author nieznanysprawiciel
@copyright Plik jest cz�ci� silnika graficznego SWEngine.

@brief Plik zawiera definicje format�w wierzcho�k�w, domy�lne layouty wierzcho�k�w
oraz inne domy�lne warto�ci zasob�w.
*/

#include "swGraphicAPI/Resources/MeshResources.h"


bool		operator==( const DirectX::XMFLOAT2& vertex1, const DirectX::XMFLOAT2& vertex2 );
bool		operator==( const DirectX::XMFLOAT3& vertex1, const DirectX::XMFLOAT3& vertex2 );
bool		operator<( const DirectX::XMFLOAT2& vertex1, const DirectX::XMFLOAT2& vertex2 );
bool		operator<( const DirectX::XMFLOAT3& vertex1, const DirectX::XMFLOAT3& vertex2 );
bool		operator<( const VertexNormalTexCoord& vertex1, const VertexNormalTexCoord& vertex2 );



//-------------------------------------------------------------------------------//
//	definicje wierzcho�k�w


namespace sw
{


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

	// Nazwy dla domy�lnych shader�w i materia�u
	static const wchar_t*	DEFAULT_MATERIAL_STRING;						///< Neutralny materia�.
	static const wchar_t*	DEFAULT_VERTEX_SHADER_STRING;					///< Shader bez obs�ugi tekstur.
	static const wchar_t*	DEFAULT_PIXEL_SHADER_STRING;					///< Shader bez obs�ugi tekstur.
	static const wchar_t*	DEFAULT_TEX_DIFFUSE_PIXEL_SHADER_PATH;			///< Shader z obs�ug� tesktury diffuse.
	static const wchar_t*	DEFAULT_COORD_COLOR_PIXEL_SHADER_PATH;			///< Shader do generowania lightmap.
	static const wchar_t*	DEFAULT_COORD_COLOR_VERTEX_SHADER_PATH;			///< Shader do generowania lightmap.
	static const wchar_t*	DEFAULT_LIGHTMAP_PIXEL_SHADER_PATH;				///< Shader u�ywaj�cy materia�u i lightmapy.

	// Domy�lne nazwy funkcji w vertex i pixel shaderze
	static const char*		DEFAULT_VERTEX_SHADER_ENTRY;					///<Domy�lna nazwa funkcji, od kt�rej zaczyna si� wykonanie vertex shadera
	static const char*		DEFAULT_PIXEL_SHADER_ENTRY;						///<Domy�lna nazwa funkcji, od kt�rej zaczyna si� wykonanie pixel shadera


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

