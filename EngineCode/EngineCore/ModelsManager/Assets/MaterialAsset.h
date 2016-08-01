#pragma once
/**
@file MaterialAsset.h
@author nieznanysprawiciel
@copyright Plik jest cz�ci� silnika graficznego SWEngine.
*/


#include "GraphicAPI/MeshResources.h"

#include <vector>

/// Maksymalna liczba tekstur w materiale.
#define MAX_MATERIAL_TEXTURES		5

/**@defgroup Assets Assety

@brief Wysokopoziomowe zasoby.

@ingroup ResourcesManagment
*/


/**@defgroup Materials Materia�y

@ingroup Assets
*/


/**@brief Layout bufora materia�u w klasie @ref MaterialAsset.

@ingroup Materials*/
struct PhongMaterialData
{
	DirectX::XMFLOAT4		Diffuse;		///< Kana� rozpraszania. Komponent w wektora jest kana�em alfa dla ca��go materia�u.
	DirectX::XMFLOAT4		Ambient;		///< Kana� ambient. Komponent w niezu�ywany.
	DirectX::XMFLOAT4		Specular;		///< Kana� odbicia zwierciadlanego. Komponent w niezu�ywany.
	DirectX::XMFLOAT3		Emissive;		///< Kolor materia�u nieo�wietlonego. Komponent w niezu�ywany.
	float					Power;			///< Wyk�adnik u�ywany przy obliczaniu odbicia zwierciadlanego.
};


/**@brief Struktura opisuj�ca dodatkowe bufory sta�ych dla shader�w.
@ingroup Materials*/
struct AdditionalBufferInfo
{
	ShaderType		Type;			///< Type shadera, do kt�rego ma zosta� zbindowany bufor.
	uint32			BufferSize;		///< Rozmiar bufora w bajtach.
};



/**@brief Obiekt opisuj�cy materia�.

Obiekt przechowuje bufor zawieraj�cy dane materia�u, wska�niki na u�ywane
shadery, tekstury oraz deklaruje dodatkowe bufory, kt�re powinny si� znale�� w klasie @ref DynamicMeshActor.

@ingroup Assets
@ingroup Materials
*/
class MaterialAsset : public ResourceObject
{
	RTTR_ENABLE( ResourceObject );
	RTTR_REGISTRATION_FRIEND
private:
	
	BufferObject*		m_materialBuffer;						///< Bufor na karcie graficznej zawieraj�cy dane materia�u.
	VertexShader*		m_vertexShader;							///< Vertex shader.
	PixelShader*		m_pixelShader;							///< Pixel shader.
	TextureObject*		m_textures[ MAX_MATERIAL_TEXTURES ];	///< Zbi�r tekstur per materia�.
	
	GeometryShader*		m_geometryShader;		///< Geometry shader.
	ControlShader*		m_controlShader;		///< Shader wywo�ywany przed etapem tesselacji.
	EvaluationShader*	m_evaluationShader;		///< Shader wywo�ywany po etapie tesselacji.
	
	///@name Descriptor
	///@{
	std::vector< AdditionalBufferInfo >		m_additionalBuffers;	///< Opis dodatkowych bufor�w, jakie musi posiada� aktor.

	///@}

public:
	MaterialAsset();

private:
	~MaterialAsset();
};

