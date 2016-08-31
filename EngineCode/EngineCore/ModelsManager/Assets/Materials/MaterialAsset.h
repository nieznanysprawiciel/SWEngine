#pragma once
/**
@file MaterialAsset.h
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.
*/


#include "GraphicAPI/MeshResources.h"
#include "ShadingModelData.h"

#include <vector>

/// Maksymalna liczba tekstur w materiale.
#define MAX_MATERIAL_TEXTURES		5

/**@defgroup Assets Assety

@brief Wysokopoziomowe zasoby.

@ingroup ResourcesManagment
*/


/**@defgroup Materials Materia³y

@brief Klasy opisuj¹ce materia³y.

KLasa @ref MaterialAsset przechowuje g³ówne dane materia³u.

@ingroup Assets
*/



/**@brief Struktura opisuj¹ca dodatkowe bufory sta³ych dla shaderów.
@ingroup Materials*/
struct AdditionalBufferInfo
{
	ShaderType		Type;			///< Type shadera, do którego ma zostaæ zbindowany bufor.
	uint32			BufferSize;		///< Rozmiar bufora w bajtach.
};


/**@brief Deskryptor materia³u.
@ingroup Materials*/
struct MaterialInfo
{
	std::vector< AdditionalBufferInfo >		m_additionalBuffers;	///< Opis dodatkowych buforów, jakie musi posiadaæ aktor.
	std::string								m_materialName;			///< Nazwa materia³u. Mo¿e to byæ œcie¿ka do pliku z którego powsta³.
	ShadingModelBase*						m_shadingData;			///< Przechowuje parametry modelu cieniowania. @see ShadingModelData
};


/**@brief Obiekt opisuj¹cy materia³.

Obiekt przechowuje bufor zawieraj¹cy dane materia³u, wskaŸniki na u¿ywane
shadery, tekstury oraz deklaruje dodatkowe bufory, które powinny siê znaleŸæ w klasie @ref StaticActor.

@ingroup Assets
@ingroup Materials
*/
class MaterialAsset : public ResourceObject
{
	RTTR_ENABLE( ResourceObject );
	RTTR_REGISTRATION_FRIEND
private:
	
	ResourcePtr< BufferObject >		m_materialBuffer;						///< Bufor na karcie graficznej zawieraj¹cy dane materia³u.
	ResourcePtr< VertexShader >		m_vertexShader;							///< Vertex shader.
	ResourcePtr< PixelShader >		m_pixelShader;							///< Pixel shader.
	ResourcePtr< TextureObject >	m_textures[ MAX_MATERIAL_TEXTURES ];	///< Zbiór tekstur per materia³.
	
	ResourcePtr< GeometryShader >	m_geometryShader;		///< Geometry shader.
	ResourcePtr< ControlShader >	m_controlShader;		///< Shader wywo³ywany przed etapem tesselacji.
	ResourcePtr< EvaluationShader >	m_evaluationShader;		///< Shader wywo³ywany po etapie tesselacji.
	

	MaterialInfo					m_descriptor;			///< Deskryptor materia³y.

public:
	MaterialAsset();

private:
	~MaterialAsset();
};

