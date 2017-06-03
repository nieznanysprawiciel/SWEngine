/**
@file MaterialAsset.cpp
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.
*/
#include "EngineCore/stdafx.h"

#include "MaterialAsset.h"
#include "EngineCore/ModelsManager/Assets/Materials/PhongMaterialData.h"

#include "swCommonLib/Common/Converters.h"

#include "swCommonLib/Common/MemoryLeaks.h"


RTTR_REGISTRATION
{


	rttr::registration::class_< sw::AdditionalBufferInfo >( "AdditionalBufferInfo" )
		.property( "ShaderType", &sw::AdditionalBufferInfo::ShaderType )
		.property( "BufferSize", &sw::AdditionalBufferInfo::BufferSize )
		.property( "BufferType", &sw::AdditionalBufferInfo::GetBufferType, &sw::AdditionalBufferInfo::SetBufferType );

	rttr::registration::class_< sw::MaterialInfo >( "MaterialInfo" )
		.property( "Name", &sw::MaterialInfo::MaterialName )
		.property( "AdditionalBuffers", &sw::MaterialInfo::AdditionalBuffers )
		.property( "ShadingData", &sw::MaterialInfo::ShadingData );

	rttr::registration::class_< sw::MaterialAsset >( "MaterialAsset" )
		.property( "VertexShader", &sw::MaterialAsset::m_vertexShader )
		.property( "PixelShader", &sw::MaterialAsset::m_pixelShader )
		.property( "GeometryShader", &sw::MaterialAsset::m_geometryShader )
		.property( "TesselationControlShader", &sw::MaterialAsset::m_controlShader )
		.property( "TesselationEvaluationShader", &sw::MaterialAsset::m_evaluationShader )
		.property_readonly( "DiffuseTexture", &sw::MaterialAsset::GetTexture1 )
		.property_readonly( "SpecularTexture", &sw::MaterialAsset::GetTexture2 )
		.property_readonly( "AmbientTexture", &sw::MaterialAsset::GetTexture3 )
		.property_readonly( "NormalMap", &sw::MaterialAsset::GetTexture4 )
		.property_readonly( "DisplacementMap", &sw::MaterialAsset::GetTexture5 )
		.property_readonly( "Texture6", &sw::MaterialAsset::GetTexture6 )
		.property_readonly( "Texture7", &sw::MaterialAsset::GetTexture7 )
		.property_readonly( "Texture8", &sw::MaterialAsset::GetTexture8 )
		//.property( "Textures", &MaterialAsset::m_textures )
		.property( "Descriptor", &sw::MaterialAsset::m_descriptor ) BIND_AS_PTR;

}


namespace sw
{


// ================================ //
//
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

// ================================ //
//
MaterialAsset::~MaterialAsset()
{}

/**@brief Returns material name or file name which was used to create material.*/
std::string MaterialAsset::GetResourceName() const
{
	return Convert::ToString( m_descriptor.MaterialName );
}



}	// sw

