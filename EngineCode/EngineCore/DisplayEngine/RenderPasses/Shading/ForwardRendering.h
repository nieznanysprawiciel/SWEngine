#pragma once
/**
@file ForwardRendering.h
@author nieznanysprawiciel
@copyright File is part of graphic engine SWEngine.
*/

#include "EngineCore/DisplayEngine/RenderPasses/IShadingPass.h"



namespace sw
{


/**@brief Implements forward rendering.

@ingroup RenderPasses*/
class ForwardRendering : public IShadingPass
{
private:
protected:

	CameraActor*						m_mainCamera;
	ResourcePtr< RenderTargetObject >	m_mainTarget;

	ResourcePtr< BlendingState >		m_blendingState;
	ResourcePtr< RasterizerState >		m_rasterizer;
	ResourcePtr< DepthStencilState >	m_depthState;

public:
	explicit		ForwardRendering	();
	virtual			~ForwardRendering	();


	// Inherited via IShadingPass
	virtual bool	PreRender		( IRenderer* renderer, RenderContext& context ) override;
	virtual void	Render			( IRenderer* renderer, RenderContext& context, Size rangeStart, Size rangeEnd ) override;
	virtual void	PostRender		( IRenderer* renderer, RenderContext& context ) override;
	virtual void	NestedPasses	( std::vector< Ptr< IRenderPass > >& passes ) override;

	virtual void			SetMainCamera		( CameraActor* camera ) override;
	virtual CameraActor*	GetMainCamera		() override;

	virtual void			SetMainRenderTarget	( const ResourcePtr< RenderTargetObject >& target ) override;

	virtual ResourcePtr< RenderTargetObject >	GetMainRenderTarget() override;

};


}	// sw

