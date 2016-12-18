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
		.property_readonly( "DiffuseTexture", &MaterialAsset::GetTexture1 )
		.property_readonly( "SpecularTexture", &MaterialAsset::GetTexture2 )
		.property_readonly( "AmbientTexture", &MaterialAsset::GetTexture3 )
		.property_readonly( "NormalMap", &MaterialAsset::GetTexture4 )
		.property_readonly( "DisplacementMap", &MaterialAsset::GetTexture5 )
		.property_readonly( "Texture6", &MaterialAsset::GetTexture6 )
		.property_readonly( "Texture7", &MaterialAsset::GetTexture7 )
		.property_readonly( "Texture8", &MaterialAsset::GetTexture8 )
		//.property( "Textures", &MaterialAsset::m_textures )
		.property( "Descriptor", &MaterialAsset::m_descriptor ) BIND_AS_PTR;

}



MaterialAsset::MaterialAsset( const std::wstring& filePath, MaterialCreateData&& initData )
	: ResourceObject( WRONG_ID )
	, m_materialBuffer( std::move( initData.MaterialBuffer ) )
	, m_vertexShader( std::move( initData.Data.VertexShader ) )
	, m_pixelShader( std::move( initData.Data.PixelShader ) )
	, m_geometryShader( std::move( initData.Data.GeometryShader ) )
	, m_evaluationShader( std::move( initData.Data.TesselationEvaluationShader ) )
	, m_controlShader( std::move( initData.Data.TesselationControlShader ) )
	, m_descriptor( filePath, std::move( initData.Data.AdditionalBuffers ), std::move( initData.Data.ShadingData ) )
{
	for( int i = 0; i < MAX_MATERIAL_TEXTURES; ++i )
	{
		m_textures[ i ] = std::move( initData.Data.Textures[ i ] );
	}
}


MaterialAsset::~MaterialAsset()
{ }

/**@brief Returns material name or file name which was used to create material.*/
std::string MaterialAsset::GetResourceName() const
{
	return Convert::ToString( m_descriptor.MaterialName );
}




