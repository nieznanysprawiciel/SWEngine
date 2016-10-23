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



/**@brief */
class DefaultLogic : public IRenderingLogicPass
{
private:
protected:

	std::vector< Ptr< IShadowPass > >		m_shadowPass;
	std::vector< Ptr< IRenderPass > >		m_environmentalPass;
	std::vector< Ptr< IRenderPass > >		m_customPass;
	std::vector< Ptr< IShadingPass > >		m_shadingPass;

	std::vector< Ptr< IPostprocessingPass > >	m_postProcessing;

public:
	explicit			DefaultLogic() = default;
						~DefaultLogic() = default;



	// Inherited via IRenderingLogicPass
	virtual void		PreRender		( IRenderer* renderer, RenderContext& context ) override;
	virtual void		Render			( IRenderer* renderer, RenderContext& context, Size rangeStart, Size rangeEnd ) override;
	virtual void		PostRender		( IRenderer* renderer, RenderContext& context ) override;

};
