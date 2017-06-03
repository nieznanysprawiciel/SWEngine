#pragma once
/**
@file MaterialAsset.h
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.
*/

#include "swCommonLib/Common/ObjectDeleter.h"

#include "swGraphicAPI/Resources/MeshResources.h"
#include "ShadingModelData.h"
#include "MaterialAssetInitData.h"
#include "MaterialInfo.h"
#include "EngineCore/ModelsManager/Assets/PipelineBinding.h"

#include <vector>


namespace sw
{


/**@defgroup Assets Assets

@brief High level engine resources.

@ingroup ResourcesManagment
*/


/**@defgroup Materials Materials

@brief Classes describing materials.

Class @ref MaterialAsset is main material class used by engine.

@ingroup Assets
*/




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

		friend ObjectDeleter< MaterialAsset >;
private:

	ResourcePtr< BufferObject >		m_materialBuffer;						///< Bufor na karcie graficznej zawieraj¹cy dane materia³u.
	ResourcePtr< VertexShader >		m_vertexShader;							///< Vertex shader.
	ResourcePtr< PixelShader >		m_pixelShader;							///< Pixel shader.
	ResourcePtr< TextureObject >	m_textures[ MAX_MATERIAL_TEXTURES ];	///< Zbiór tekstur per materia³.

	ResourcePtr< GeometryShader >	m_geometryShader;		///< Geometry shader.
	ResourcePtr< ControlShader >	m_controlShader;		///< Shader wywo³ywany przed etapem tesselacji.
	ResourcePtr< EvaluationShader >	m_evaluationShader;		///< Shader wywo³ywany po etapie tesselacji.


	MaterialInfo					m_descriptor;			///< Deskryptor materia³u.

public:
	explicit			MaterialAsset		( const std::wstring& filePath, MaterialCreateData&& initData );

	const ResourcePtr< BufferObject >&		GetMaterialBuffer		() const { return m_materialBuffer; }
	const ResourcePtr< VertexShader >&		GetVertexShader			() const { return m_vertexShader; }
	const ResourcePtr< PixelShader >&		GetPixelShader			() const { return m_pixelShader; }
	const ResourcePtr< GeometryShader >&	GetGeometryShader		() const { return m_geometryShader; }
	const ResourcePtr< ControlShader >&		GetTessControlShader	() const { return m_controlShader; }
	const ResourcePtr< EvaluationShader >&	GetTessEvaluationShader	() const { return m_evaluationShader; }

	const ResourcePtr< TextureObject >&		GetTexture		( int index ) const { return m_textures[ index ]; }
	const MaterialInfo&						GetDescriptor	() const { return m_descriptor; }

	virtual std::string						GetResourceName	() const override;

private:
	~MaterialAsset();

	// Temporary textures getters for properties.
	TextureObject*		GetTexture1() const { return m_textures[ 0 ].Ptr(); }
	TextureObject*		GetTexture2() const { return m_textures[ 1 ].Ptr(); }
	TextureObject*		GetTexture3() const { return m_textures[ 2 ].Ptr(); }
	TextureObject*		GetTexture4() const { return m_textures[ 3 ].Ptr(); }
	TextureObject*		GetTexture5() const { return m_textures[ 4 ].Ptr(); }
	TextureObject*		GetTexture6() const { return m_textures[ 5 ].Ptr(); }
	TextureObject*		GetTexture7() const { return m_textures[ 6 ].Ptr(); }
	TextureObject*		GetTexture8() const { return m_textures[ 7 ].Ptr(); }
};

}	// sw
