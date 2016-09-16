/**
@file TextureBinding.cpp
@author nieznanysprawiciel
@copyright File is part of graphic engine SWEngine.
*/

#include "EngineCore/stdafx.h"
#include "TextureBinding.h"


RTTR_REGISTRATION
{
	rttr::registration::class_< TextureBinding >( "TextureBinding" )
		.property( "Texture", &TextureBinding::m_texture )
		.property( "BindToPixelShader", &TextureBinding::IsBoundPixelShader, &TextureBinding::BindPixelShader )
		.property( "BindToVertexShader", &TextureBinding::IsBoundVertexShader, &TextureBinding::BindVertexShader )
		.property( "BindToGeometryShader", &TextureBinding::IsBoundGemetryShader, &TextureBinding::BindGeometryShader )
		.property( "BindToTesselationControlShader", &TextureBinding::IsBoundTesselationControlShader, &TextureBinding::BindTesselationControlShader )
		.property( "BindToTesselationEvaluationShader", &TextureBinding::IsBoundTesselationEvaluationShader, &TextureBinding::BindTesselationEvaluationShader );
}


TextureBinding::TextureBinding()
	:	m_flags( (uint8)ShaderType::PixelShader )
{ }

TextureBinding::TextureBinding( TextureBindingFlag flags )
	:	m_flags( flags )
{ }

TextureBinding::TextureBinding( TextureObject* tex )
	:	m_texture( tex )
	,	m_flags( (uint8)ShaderType::PixelShader )
{ }

TextureBinding::TextureBinding( const ResourcePtr< TextureObject >& tex )
	:	m_texture( tex )
	,	m_flags( (uint8)ShaderType::PixelShader )
{ }

TextureBinding::TextureBinding( ResourcePtr< TextureObject >&& tex )
	:	m_texture( std::move( tex ) )
	,	m_flags( (uint8)ShaderType::PixelShader )
{ }

