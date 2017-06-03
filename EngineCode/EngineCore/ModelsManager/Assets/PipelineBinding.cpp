/**
@file PipelineBinding.cpp
@author nieznanysprawiciel
@copyright File is part of graphic engine SWEngine.
*/

#include "EngineCore/stdafx.h"
#include "PipelineBinding.h"


RTTR_REGISTRATION
{
	rttr::registration::class_< sw::Binding< TextureObject > >( "TextureBinding" )
		.property( "Texture", &sw::Binding< TextureObject >::m_resource )
		.property( "BindToPixelShader", &sw::Binding< TextureObject >::IsBoundPixelShader, &sw::Binding< TextureObject >::BindPixelShader )
		.property( "BindToVertexShader", &sw::Binding< TextureObject >::IsBoundVertexShader, &sw::Binding< TextureObject >::BindVertexShader )
		.property( "BindToGeometryShader", &sw::Binding< TextureObject >::IsBoundGemetryShader, &sw::Binding< TextureObject >::BindGeometryShader )
		.property( "BindToTesselationControlShader", &sw::Binding< TextureObject >::IsBoundTesselationControlShader, &sw::Binding< TextureObject >::BindTesselationControlShader )
		.property( "BindToTesselationEvaluationShader", &sw::Binding< TextureObject >::IsBoundTesselationEvaluationShader, &sw::Binding< TextureObject >::BindTesselationEvaluationShader );

	rttr::registration::class_< sw::Binding< BufferObject > >( "BufferBinding" )
		.property( "Buffer", &sw::Binding< BufferObject >::m_resource )
		.property( "BindToPixelShader", &sw::Binding< BufferObject >::IsBoundPixelShader, &sw::Binding< BufferObject >::BindPixelShader )
		.property( "BindToVertexShader", &sw::Binding< BufferObject >::IsBoundVertexShader, &sw::Binding< BufferObject >::BindVertexShader )
		.property( "BindToGeometryShader", &sw::Binding< BufferObject >::IsBoundGemetryShader, &sw::Binding< BufferObject >::BindGeometryShader )
		.property( "BindToTesselationControlShader", &sw::Binding< BufferObject >::IsBoundTesselationControlShader, &sw::Binding< BufferObject >::BindTesselationControlShader )
		.property( "BindToTesselationEvaluationShader", &sw::Binding< BufferObject >::IsBoundTesselationEvaluationShader, &sw::Binding< BufferObject >::BindTesselationEvaluationShader );
}


