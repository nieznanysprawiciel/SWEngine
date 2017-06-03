#pragma once
/**
@file MaterialAssetInitData.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/


#include "swCommonLib/Common/TypesDefinitions.h"

#include "swGraphicAPI/Resources/MeshResources.h"
#include "ShadingModelData.h"
#include "MaterialInfo.h"

#include <vector>


namespace sw
{



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
	UPtr< ShadingModelBase >				ShadingData;			///< Initializes buffer with shadeing model data. @see ShadingModelData

// ================================ //
// This should be generated automatically, but Visual studio 2013 can't :(
	MaterialInitData	() {}
	MaterialInitData	( MaterialInitData&& other );
	void operator=		( MaterialInitData&& other );
};

/**@brief Struct contains data needed to initialize material.

For internal use by @ref AssetsManager. Use MaterialInitData instead.
This structure gets it's own @ref BufferObject for storing shading data.
You can create materials sharing same buffer but updating one material shading data would
cause updating other materials without changing ShadingData.

@ingroup Materials*/
struct MaterialCreateData
{
	MaterialInitData				Data;
	ResourcePtr< BufferObject >		MaterialBuffer;
};


//====================================================================================//
//			Implementation	
//====================================================================================//

#define MOVE_FIELD( param ) param = std::move( other.##param );

inline MaterialInitData::MaterialInitData( MaterialInitData&& other )
{
	MOVE_FIELD( VertexShader );
	MOVE_FIELD( PixelShader );
	MOVE_FIELD( GeometryShader );
	MOVE_FIELD( TesselationControlShader );
	MOVE_FIELD( TesselationEvaluationShader );
	MOVE_FIELD( AdditionalBuffers );
	MOVE_FIELD( ShadingData );

	for( int i = 0; i < MAX_MATERIAL_TEXTURES; ++i )
		Textures[ i ] = std::move( other.Textures[ i ] );
}

inline void		MaterialInitData::operator=( MaterialInitData&& other )
{
	MOVE_FIELD( VertexShader );
	MOVE_FIELD( PixelShader );
	MOVE_FIELD( GeometryShader );
	MOVE_FIELD( TesselationControlShader );
	MOVE_FIELD( TesselationEvaluationShader );
	MOVE_FIELD( AdditionalBuffers );
	MOVE_FIELD( ShadingData );

	for( int i = 0; i < MAX_MATERIAL_TEXTURES; ++i )
		Textures[ i ] = std::move( other.Textures[ i ] );
}

}	// sw

