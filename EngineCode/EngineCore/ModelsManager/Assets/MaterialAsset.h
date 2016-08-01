#pragma once
/**
@file MaterialAsset.h
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.
*/


#include "GraphicAPI/MeshResources.h"

#include <vector>


#define MAX_MATERIAL_TEXTURES 5

/**@defgroup Assets Assety

@brief Wysokopoziomowe zasoby.

@ingroup ResourcesManagment
*/



/**@brief Struktura opisuj¹ca dodatkowe bufory sta³ych dla shaderów.*/
struct AdditionalBufferInfo
{
	ShaderType		Type;
	uint32			BufferSize;
};


/**@brief Deskrpytor materia³u.

@ingroup Assets*/
struct MaterialDescriptor
{
	std::vector< AdditionalBufferInfo >		AdditionalBuffers;
};




/**@brief Obiekt opisuj¹cy materia³.

Obiekt przechowuje bufor zawieraj¹cy dane materia³u oraz wskaŸniki na u¿ywane
shadery. W deskryptorze mo¿na deklarowaæ dodatkowe bufory wymagane przez materia³.

@ingroup Assets*/
class MaterialAsset : public ResourceObject
{
private:
	
	BufferObject*		m_materialBuffer;
	VertexShader*		m_vertexShader;
	PixelShader*		m_pixelShader;
	TextureObject*		m_textures[ MAX_MATERIAL_TEXTURES ];
	
	GeometryShader*		m_geometryShader;
	ControlShader*		m_controlShader;
	EvaluationShader*	m_evaluationShader;
	MaterialDescriptor*	m_descriptor;

public:
	MaterialAsset();
	~MaterialAsset();
};

