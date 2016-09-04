#pragma once

#include "GraphicAPI/MeshResources.h"
#include "ShadingModelData.h"
#include "MaterialInfo.h"

#include <vector>


/**@brief Struct contains data used to initialize material.
@ingroup Materials*/
struct MaterialAssetInitData
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
