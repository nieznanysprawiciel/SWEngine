#pragma once
/**
@file MaterialAsset.h
@author nieznanysprawiciel
@copyright Plik jest cz�ci� silnika graficznego SWEngine.
*/

#include "Common/ObjectDeleter.h"

#include "GraphicAPI/MeshResources.h"
#include "ShadingModelData.h"
#include "MaterialAssetInitData.h"
#include "MaterialInfo.h"
#include "EngineCore/ModelsManager/Assets/PipelineBinding.h"

#include <vector>



/**@defgroup Assets Assets

@brief High level engine resources.

@ingroup ResourcesManagment
*/


/**@defgroup Materials Materials

@brief Classes describing materials.

Class @ref MaterialAsset is main material class used by engine.

@ingroup Assets
*/




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

	friend ObjectDeleter< MaterialAsset >;
private:
	
	ResourcePtr< BufferObject >		m_materialBuffer;						///< Bufor na karcie graficznej zawieraj�cy dane materia�u.
	ResourcePtr< VertexShader >		m_vertexShader;							///< Vertex shader.
	ResourcePtr< PixelShader >		m_pixelShader;							///< Pixel shader.
	ResourcePtr< TextureObject >	m_textures[ MAX_MATERIAL_TEXTURES ];	///< Zbi�r tekstur per materia�.
	
	ResourcePtr< GeometryShader >	m_geometryShader;		///< Geometry shader.
	ResourcePtr< ControlShader >	m_controlShader;		///< Shader wywo�ywany przed etapem tesselacji.
	ResourcePtr< EvaluationShader >	m_evaluationShader;		///< Shader wywo�ywany po etapie tesselacji.
	

	MaterialInfo					m_descriptor;			///< Deskryptor materia�u.

public:
	explicit			MaterialAsset		( const std::wstring& filePath, MaterialCreateData&& initData );

	const ResourcePtr< BufferObject >&		GetMaterialBuffer		() const		{ return m_materialBuffer;  }
	const ResourcePtr< VertexShader >&		GetVertexShader			() const		{ return m_vertexShader; }
	const ResourcePtr< PixelShader >&		GetPixelShader			() const		{ return m_pixelShader; }
	const ResourcePtr< GeometryShader >&	GetGeometryShader		() const		{ return m_geometryShader; }
	const ResourcePtr< ControlShader >&		GetTessControlShader	() const		{ return m_controlShader; }
	const ResourcePtr< EvaluationShader >&	GetTessEvaluationShader	() const		{ return m_evaluationShader; }

	const ResourcePtr< TextureObject >&		GetTexture		( int index ) const		{ return m_textures[index]; }
	const MaterialInfo&						GetDescriptor	() const				{ return m_descriptor; }

	virtual std::string						GetResourceName	() const override;

private:
	~MaterialAsset();
};

