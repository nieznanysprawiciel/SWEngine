#pragma once
/**
@file MaterialAsset.h
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.
*/


#include "GraphicAPI/MeshResources.h"

#include <vector>

/// Maksymalna liczba tekstur w materiale.
#define MAX_MATERIAL_TEXTURES		5

/**@defgroup Assets Assety

@brief Wysokopoziomowe zasoby.

@ingroup ResourcesManagment
*/


/**@defgroup Materials Materia³y

@ingroup Assets
*/


/**@brief Layout bufora materia³u w klasie @ref MaterialAsset.

@ingroup Materials*/
struct PhongMaterialData
{
	DirectX::XMFLOAT4		Diffuse;		///< Kana³ rozpraszania. Komponent w wektora jest kana³em alfa dla ca³êgo materia³u.
	DirectX::XMFLOAT4		Ambient;		///< Kana³ ambient. Komponent w niezu¿ywany.
	DirectX::XMFLOAT4		Specular;		///< Kana³ odbicia zwierciadlanego. Komponent w niezu¿ywany.
	DirectX::XMFLOAT3		Emissive;		///< Kolor materia³u nieoœwietlonego. Komponent w niezu¿ywany.
	float					Power;			///< Wyk³adnik u¿ywany przy obliczaniu odbicia zwierciadlanego.
};


/**@brief Struktura opisuj¹ca dodatkowe bufory sta³ych dla shaderów.
@ingroup Materials*/
struct AdditionalBufferInfo
{
	ShaderType		Type;			///< Type shadera, do którego ma zostaæ zbindowany bufor.
	uint32			BufferSize;		///< Rozmiar bufora w bajtach.
};



/**@brief Obiekt opisuj¹cy materia³.

Obiekt przechowuje bufor zawieraj¹cy dane materia³u, wskaŸniki na u¿ywane
shadery, tekstury oraz deklaruje dodatkowe bufory, które powinny siê znaleŸæ w klasie @ref DynamicMeshActor.

@ingroup Assets
@ingroup Materials
*/
class MaterialAsset : public ResourceObject
{
	RTTR_ENABLE( ResourceObject );
	RTTR_REGISTRATION_FRIEND
private:
	
	BufferObject*		m_materialBuffer;						///< Bufor na karcie graficznej zawieraj¹cy dane materia³u.
	VertexShader*		m_vertexShader;							///< Vertex shader.
	PixelShader*		m_pixelShader;							///< Pixel shader.
	TextureObject*		m_textures[ MAX_MATERIAL_TEXTURES ];	///< Zbiór tekstur per materia³.
	
	GeometryShader*		m_geometryShader;		///< Geometry shader.
	ControlShader*		m_controlShader;		///< Shader wywo³ywany przed etapem tesselacji.
	EvaluationShader*	m_evaluationShader;		///< Shader wywo³ywany po etapie tesselacji.
	
	///@name Descriptor
	///@{
	std::vector< AdditionalBufferInfo >		m_additionalBuffers;	///< Opis dodatkowych buforów, jakie musi posiadaæ aktor.

	///@}

public:
	MaterialAsset();

private:
	~MaterialAsset();
};

