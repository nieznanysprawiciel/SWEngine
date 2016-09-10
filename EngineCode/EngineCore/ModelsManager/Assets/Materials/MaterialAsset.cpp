/**
@file MaterialAsset.cpp
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.
*/


#include "EngineCore/stdafx.h"
#include "MaterialAsset.h"
#include "EngineCore/ModelsManager/Assets/Materials/PhongMaterialData.h"

#include "Common/Converters.h"

#include "Common/MemoryLeaks.h"


RTTR_REGISTRATION
{


	rttr::registration::class_< AdditionalBufferInfo >( "AdditionalBufferInfo" )
		.property( "ShaderType", &AdditionalBufferInfo::ShaderType )
		.property( "BufferSize", &AdditionalBufferInfo::BufferSize )
		.property( "BufferType", &AdditionalBufferInfo::GetBufferType, &AdditionalBufferInfo::SetBufferType );

	rttr::registration::class_< MaterialInfo >( "MaterialInfo" )
		.property( "Name", &MaterialInfo::MaterialName )
		.property( "AdditionalBuffers", &MaterialInfo::AdditionalBuffers )
		.property( "ShadingData", &MaterialInfo::ShadingData );

	rttr::registration::class_< MaterialAsset >( "MaterialAsset" )
		.property( "VertexShader", &MaterialAsset::m_vertexShader )
		.property( "PixelShader", &MaterialAsset::m_pixelShader )
		.property( "GeometryShader", &MaterialAsset::m_geometryShader )
		.property( "TesselationControlShader", &MaterialAsset::m_controlShader )
		.property( "TesselationEvaluationShader", &MaterialAsset::m_evaluationShader )
		.property( "Descriptor", &MaterialAsset::m_descriptor );

}



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
	return Convert::ToString( m_descriptor.MaterialName );
}




