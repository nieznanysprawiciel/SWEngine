#include "EngineCore/stdafx.h"
/**
@file DefaultLogicPass.cpp
@author nieznanysprawiciel
@copyright File is part of graphic engine SWEngine.
*/


#include "DefaultLogic.h"
#include "EngineCore/DisplayEngine/RenderPasses/RenderPassFactory.h"



namespace sw
{


// ================================ //
//
DefaultLogic::DefaultLogic()
{
	m_shadingPass = s_renderPassFactory->CreateDefaultShadingPass();
}

// ================================ //
//
bool		DefaultLogic::PreRender		( IRenderer* renderer, RenderContext& context )
{
	return false;
}

// ================================ //
//
void		DefaultLogic::Render		( IRenderer* renderer, RenderContext& context, Size rangeStart, Size rangeEnd )
{}

// ================================ //
//
void		DefaultLogic::PostRender	( IRenderer* renderer, RenderContext& context )
{}

// ================================ //
//
void		DefaultLogic::NestedPasses( std::vector< Ptr< IRenderPass > >& passes )
{
	FillWithNestedPasses( passes, m_shadowPass );
	FillWithNestedPasses( passes, m_environmentalPass );
	FillWithNestedPasses( passes, m_customPass );

	passes.push_back( m_shadingPass );
	m_shadingPass->NestedPasses( passes );

	FillWithNestedPasses( passes, m_postProcessing );
}

// ================================ //
//
void		DefaultLogic::SetMainCamera	( CameraActor* camera )
{
	m_shadingPass->SetMainCamera( camera );
}

// ================================ //
//
CameraActor*	DefaultLogic::GetMainCamera()
{
	return m_shadingPass->GetMainCamera();
}

// ================================ //
//
void		DefaultLogic::SetMainRenderTarget	( const ResourcePtr< RenderTargetObject >& target )
{
	m_shadingPass->SetMainRenderTarget( target );
}

// ================================ //
//
ResourcePtr< RenderTargetObject >			DefaultLogic::GetMainRenderTarget()
{
	return m_shadingPass->GetMainRenderTarget();
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

}	// sw

