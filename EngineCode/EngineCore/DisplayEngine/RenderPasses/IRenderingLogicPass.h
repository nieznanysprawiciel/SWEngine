#pragma once
/**
@file IRenderingLogicPass.h
@author nieznanysprawiciel
@copyright File is part of graphic engine SWEngine.
*/


#include "EngineCore/DisplayEngine/RenderPasses/IRenderPass.h"
#include "EngineCore/DisplayEngine/RenderPasses/IShadowPass.h"
#include "EngineCore/DisplayEngine/RenderPasses/IShadingPass.h"
#include "EngineCore/DisplayEngine/RenderPasses/IPostProcessingPass.h"

/**@brief Interface for rendering logic.

Rendering logic groups all passes invoked during rendering process.

@ingroup RenderPasses*/
class IRenderingLogicPass : public IRenderPass
{
private:
protected:
public:
	explicit		IRenderingLogicPass() = default;
					~IRenderingLogicPass() = default;


	virtual void			SetMainCamera	( CameraActor* camera )		= 0;
	virtual CameraActor*	GetMainCamera	()							= 0;

	virtual void			SetMainRenderTarget	( const ResourcePtr< RenderTargetObject >& target )	= 0;
	//virtual void			SetSkyDome			( SkyDome* skyDome ) = 0;

	virtual ResourcePtr< RenderTargetObject >		GetMainRenderTarget	() = 0;

	// Nested passes
	virtual const std::vector< Ptr< IShadowPass > >&		GetShadowPasses			() = 0;
	virtual const std::vector< Ptr< IRenderPass > >&		GetEnvironmentalPasses	() = 0;
	virtual const std::vector< Ptr< IRenderPass > >&		GetCustomPasses			() = 0;

	virtual const Ptr< IShadingPass >&						GetShadingPass			() = 0;
	//virtual const Ptr< IRenderPass >&						GetSkyDomePass			() = 0;

	virtual const std::vector< Ptr< IPostprocessingPass > >&	GetPostProcessing	() = 0;
};
