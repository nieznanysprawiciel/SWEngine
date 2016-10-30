#include "EngineCore/stdafx.h"
#include "HosekSkyDomePass.h"





HosekSkyDomePass::HosekSkyDomePass	( SkyDome* dome )
	:	m_skyDome( dome )
{}


/**@brief Renders SkyDome.*/
bool	HosekSkyDomePass::PreRender		( IRenderer* renderer, RenderContext& context )
{
	return false;
}


/**@brief Empty*/
void	HosekSkyDomePass::Render		( IRenderer* renderer, RenderContext& context, Size rangeStart, Size rangeEnd )
{}

/**@brief Empty*/
void HosekSkyDomePass::PostRender( IRenderer * renderer, RenderContext & context )
{ }

/**@brief Empty*/
void HosekSkyDomePass::NestedPasses( std::vector<Ptr<IRenderPass>>& passes )
{ }
