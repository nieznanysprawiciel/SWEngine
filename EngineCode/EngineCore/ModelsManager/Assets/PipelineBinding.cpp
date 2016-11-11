/**
@file PipelineBinding.cpp
@author nieznanysprawiciel
@copyright File is part of graphic engine SWEngine.
*/

#include "EngineCore/stdafx.h"
#include "PipelineBinding.h"


RTTR_REGISTRATION
{
	rttr::registration::class_< Binding< TextureObject > >( "TextureBinding" )
		.property( "Texture", &Binding< TextureObject >::m_resource )
		.property( "BindToPixelShader", &Binding< TextureObject >::IsBoundPixelShader, &Binding< TextureObject >::BindPixelShader )
		.property( "BindToVertexShader", &Binding< TextureObject >::IsBoundVertexShader, &Binding< TextureObject >::BindVertexShader )
		.property( "BindToGeometryShader", &Binding< TextureObject >::IsBoundGemetryShader, &Binding< TextureObject >::BindGeometryShader )
		.property( "BindToTesselationControlShader", &Binding< TextureObject >::IsBoundTesselationControlShader, &Binding< TextureObject >::BindTesselationControlShader )
		.property( "BindToTesselationEvaluationShader", &Binding< TextureObject >::IsBoundTesselationEvaluationShader, &Binding< TextureObject >::BindTesselationEvaluationShader );

	rttr::registration::class_< Binding< BufferObject > >( "BufferBinding" )
		.property( "Buffer", &Binding< BufferObject >::m_resource )
		.property( "BindToPixelShader", &Binding< BufferObject >::IsBoundPixelShader, &Binding< BufferObject >::BindPixelShader )
		.property( "BindToVertexShader", &Binding< BufferObject >::IsBoundVertexShader, &Binding< BufferObject >::BindVertexShader )
		.property( "BindToGeometryShader", &Binding< BufferObject >::IsBoundGemetryShader, &Binding< BufferObject >::BindGeometryShader )
		.property( "BindToTesselationControlShader", &Binding< BufferObject >::IsBoundTesselationControlShader, &Binding< BufferObject >::BindTesselationControlShader )
		.property( "BindToTesselationEvaluationShader", &Binding< BufferObject >::IsBoundTesselationEvaluationShader, &Binding< BufferObject >::BindTesselationEvaluationShader );
}


