#pragma once

#include "GraphicAPI/MeshResources.h"
#include "ShadingModelData.h"
#include "MaterialInfo.h"

#include <vector>


/**@brief Struct contains data needed to initialize material.
@ingroup Materials*/
struct MaterialInitData
{
	ResourcePtr< VertexShader >				VertexShader;
	ResourcePtr< PixelShader >				PixelShader;
	ResourcePtr< GeometryShader >			GeometryShader;
	ResourcePtr< ControlShader >			TesselationControlShader;
	ResourcePtr< EvaluationShader >			TesselationEvaluationShader;
	ResourcePtr< TextureObject >			Textures[ MAX_MATERIAL_TEXTURES ];

	std::vector< AdditionalBufferInfo >		AdditionalBuffers;		///< Additional buffers which should be provided by actor.
	ShadingModelBase*						ShadingData;			///< Initializes buffer with shadeing model data. @see ShadingModelData
};

/**@brief Struct contains data needed to initialize material.

For internal use by @ref ModelsManager. Use MaterialInitData instead.
@ingroup Materials*/
struct MaterialCreateData
{
	MaterialInitData				Data;
	ResourcePtr< BufferObject >		MaterialBuffer;
};

