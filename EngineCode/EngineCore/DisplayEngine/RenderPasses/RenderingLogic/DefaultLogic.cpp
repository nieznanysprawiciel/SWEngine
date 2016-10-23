#include "EngineCore/stdafx.h"
/**
@file DefaultLogicPass.cpp
@author nieznanysprawiciel
@copyright File is part of graphic engine SWEngine.
*/


#include "DefaultLogic.h"


// ================================ //
//
bool		DefaultLogic::PreRender		( IRenderer* renderer, RenderContext& context )
{ return false; }

// ================================ //
//
void		DefaultLogic::Render		( IRenderer* renderer, RenderContext& context, Size rangeStart, Size rangeEnd )
{ }

// ================================ //
//
void		DefaultLogic::PostRender	( IRenderer* renderer, RenderContext& context )
{ }

// ================================ //
//
void		DefaultLogic::NestedPasses( std::vector< Ptr< IRenderPass > >& passes )
{
	for( auto& pass : m_shadowPass )
		passes.push_back( pass );

	for( auto& pass : m_environmentalPass )
		passes.push_back( pass );

	for( auto& pass : m_customPass )
		passes.push_back( pass );

	passes.push_back( m_shadingPass );

	for( auto& pass : m_postProcessing )
		passes.push_back( pass );
}

// ================================ //
//
void		DefaultLogic::SetMainCamera	( CameraActor* camera )
{
	//m_shadingPass->
}

// ================================ //
//
CameraActor*	DefaultLogic::GetMainCamera()
{
	return nullptr;
}

// ================================ //
//
void		DefaultLogic::SetMainRenderTarget	( const ResourcePtr< RenderTargetObject >& target )
{ }

// ================================ //
//
ResourcePtr< RenderTargetObject >			DefaultLogic::GetMainRenderTarget()
{
	return ResourcePtr<RenderTargetObject>();
}

// ================================ //
//
const std::vector< Ptr< IShadowPass > >&	DefaultLogic::GetShadowPasses()
{
	return m_shadowPass;
}

// ================================ //
//
const std::vector< Ptr< IRenderPass > >&	DefaultLogic::GetEnvironmentalPasses()
{
	return m_environmentalPass;
}

// ================================ //
//
const std::vector< Ptr< IRenderPass > >&	DefaultLogic::GetCustomPasses()
{
	return m_customPass;
}

// ================================ //
//
const Ptr< IShadingPass >&					DefaultLogic::GetShadingPass()
{
	return m_shadingPass;
}

// ================================ //
//
const std::vector< Ptr< IPostprocessingPass > >&	DefaultLogic::GetPostProcessing()
{
	return m_postProcessing;
}
