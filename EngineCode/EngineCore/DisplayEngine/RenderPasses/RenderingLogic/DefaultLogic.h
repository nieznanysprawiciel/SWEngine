#pragma once
/**
@file DefaultLogicPass.h
@author nieznanysprawiciel
@copyright File is part of graphic engine SWEngine.
*/

#include "EngineCore/DisplayEngine/RenderPasses/IRenderingLogicPass.h"

#include "EngineCore/DisplayEngine/RenderPasses/IPostProcessingPass.h"
#include "EngineCore/DisplayEngine/RenderPasses/IShadowPass.h"
#include "EngineCore/DisplayEngine/RenderPasses/IShadingPass.h"

#include <vector>



namespace sw
{


/**@brief

@ingroup RenderPasses*/
class DefaultLogic : public IRenderingLogicPass
{
private:
protected:

	std::vector< Ptr< IShadowPass > >		m_shadowPass;
	std::vector< Ptr< IRenderPass > >		m_environmentalPass;
	std::vector< Ptr< IRenderPass > >		m_customPass;

	Ptr< IShadingPass >						m_shadingPass;
	Ptr< IRenderPass >						m_skyDome;

	std::vector< Ptr< IPostprocessingPass > >	m_postProcessing;


	//CameraActor*						m_mainCamera;
	//ResourcePtr< RenderTargetObject >	m_mainTarget;

public:
	explicit			DefaultLogic	();
	virtual 			~DefaultLogic	() = default;



	// Inherited via IRenderingLogicPass
	virtual bool		PreRender		( IRenderer* renderer, RenderContext& context ) override;
	virtual void		Render			( IRenderer* renderer, RenderContext& context, Size rangeStart, Size rangeEnd ) override;
	virtual void		PostRender		( IRenderer* renderer, RenderContext& context ) override;
	virtual void		NestedPasses	( std::vector< Ptr< IRenderPass > >& passes ) override;


	virtual void			SetMainCamera			( CameraActor* camera ) override;
	virtual CameraActor*	GetMainCamera			() override;
	virtual void			SetMainRenderTarget		( const ResourcePtr< RenderTargetObject >& target ) override;

	virtual ResourcePtr< RenderTargetObject >	GetMainRenderTarget	() override;

	// Nested passes
	virtual const std::vector< Ptr< IShadowPass > >&		GetShadowPasses			() override;
	virtual const std::vector< Ptr< IRenderPass > >&		GetEnvironmentalPasses	() override;
	virtual const std::vector< Ptr< IRenderPass > >&		GetCustomPasses			() override;

	virtual const Ptr< IShadingPass >&						GetShadingPass			() override;

	virtual const std::vector< Ptr< IPostprocessingPass > >&	GetPostProcessing	() override;

};

}	// sw

