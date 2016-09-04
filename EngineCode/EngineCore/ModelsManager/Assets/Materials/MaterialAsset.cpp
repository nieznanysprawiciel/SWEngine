/**
@file MaterialAsset.cpp
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.
*/


#include "EngineCore/stdafx.h"
#include "MaterialAsset.h"

#include "EngineCore/EngineHelpers/Converters.h"

#include "Common/MemoryLeaks.h"


MaterialAsset::MaterialAsset( const std::wstring& filePath, MaterialAssetInitData&& initData )
	: ResourceObject( WRONG_ID )
	, m_vertexShader( std::move( initData.VertexShader ) )
	, m_pixelShader( std::move( initData.PixelShader ) )
	, m_geometryShader( std::move( initData.GeometryShader ) )
	, m_evaluationShader( std::move( initData.TesselationEvaluationShader ) )
	, m_controlShader( std::move( initData.TesselationControlShader ) )
	, m_descriptor( filePath, std::move( initData.AdditionalBuffers ), initData.ShadingData )
{
	for( int i = 0; i < MAX_MATERIAL_TEXTURES; ++i )
	{
		m_textures[ i ] = std::move( initData.Textures[ i ] );
	}
}


MaterialAsset::~MaterialAsset()
{ }

/**@brief Returns material name or file name which was used to create material.*/
std::string MaterialAsset::GetResourceName() const
{
	return Converters::ToString( m_descriptor.MaterialName );
}

