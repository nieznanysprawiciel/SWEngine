#pragma once
/**
@file MaterialAsset.h
@author nieznanysprawiciel
@copyright Plik jest cz�ci� silnika graficznego SWEngine.
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


/**@defgroup Materials Materia�y

@brief Klasy opisuj�ce materia�y.

KLasa @ref MaterialAsset przechowuje g��wne dane materia�u.

@ingroup Assets
*/



/**@brief Struktura opisuj�ca dodatkowe bufory sta�ych dla shader�w.
@ingroup Materials*/
struct AdditionalBufferInfo
{
	ShaderType		Type;			///< Type shadera, do kt�rego ma zosta� zbindowany bufor.
	uint32			BufferSize;		///< Rozmiar bufora w bajtach.
};


/**@brief Deskryptor materia�u.
@ingroup Materials*/
struct MaterialInfo
{
	std::vector< AdditionalBufferInfo >		m_additionalBuffers;	///< Opis dodatkowych bufor�w, jakie musi posiada� aktor.
	std::string								m_materialName;			///< Nazwa materia�u. Mo�e to by� �cie�ka do pliku z kt�rego powsta�.
	ShadingModelBase*						m_shadingData;			///< Przechowuje parametry modelu cieniowania. @see ShadingModelData
};


/**@brief Obiekt opisuj�cy materia�.

Obiekt przechowuje bufor zawieraj�cy dane materia�u, wska�niki na u�ywane
shadery, tekstury oraz deklaruje dodatkowe bufory, kt�re powinny si� znale�� w klasie @ref StaticActor.

@ingroup Assets
@ingroup Materials
*/
class MaterialAsset : public ResourceObject
{
	RTTR_ENABLE( ResourceObject );
	RTTR_REGISTRATION_FRIEND
private:
	
	ResourcePtr< BufferObject >		m_materialBuffer;						///< Bufor na karcie graficznej zawieraj�cy dane materia�u.
	ResourcePtr< VertexShader >		m_vertexShader;							///< Vertex shader.
	ResourcePtr< PixelShader >		m_pixelShader;							///< Pixel shader.
	ResourcePtr< TextureObject >	m_textures[ MAX_MATERIAL_TEXTURES ];	///< Zbi�r tekstur per materia�.
	
	ResourcePtr< GeometryShader >	m_geometryShader;		///< Geometry shader.
	ResourcePtr< ControlShader >	m_controlShader;		///< Shader wywo�ywany przed etapem tesselacji.
	ResourcePtr< EvaluationShader >	m_evaluationShader;		///< Shader wywo�ywany po etapie tesselacji.
	

	MaterialInfo					m_descriptor;			///< Deskryptor materia�y.

public:
	MaterialAsset();

private:
	~MaterialAsset();
};

