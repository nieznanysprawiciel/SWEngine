#pragma once
/**
@file MaterialAsset.h
@author nieznanysprawiciel
@copyright Plik jest cz�ci� silnika graficznego SWEngine.
*/


#include "GraphicAPI/MeshResources.h"

#include <vector>


#define MAX_MATERIAL_TEXTURES 5

/**@defgroup Assets Assety

@brief Wysokopoziomowe zasoby.

@ingroup ResourcesManagment
*/



/**@brief Struktura opisuj�ca dodatkowe bufory sta�ych dla shader�w.*/
struct AdditionalBufferInfo
{
	ShaderType		Type;
	uint32			BufferSize;
};


/**@brief Deskrpytor materia�u.

@ingroup Assets*/
struct MaterialDescriptor
{
	std::vector< AdditionalBufferInfo >		AdditionalBuffers;
};




/**@brief Obiekt opisuj�cy materia�.

Obiekt przechowuje bufor zawieraj�cy dane materia�u oraz wska�niki na u�ywane
shadery. W deskryptorze mo�na deklarowa� dodatkowe bufory wymagane przez materia�.

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

