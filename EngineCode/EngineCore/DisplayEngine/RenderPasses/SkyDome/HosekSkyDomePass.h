#pragma once
/**
@file HosekSkyDomePass.h
@author nieznanysprawiciel
@copyright File is part of graphic engine SWEngine.
*/

#include "EngineCore/DisplayEngine/RenderPasses/IRenderPass.h"
#include "EngineCore/Features/SkyDome.h"


/**@brief */
class HosekSkyDomePass : public IRenderPass
{
private:
protected:
	SkyDome*		m_skyDome;

public:
	explicit		HosekSkyDomePass	( SkyDome* dome );
					~HosekSkyDomePass	() = default;



	// Inherited via IRenderPass
	virtual bool	PreRender		( IRenderer* renderer, RenderContext& context ) override;
	virtual void	Render			( IRenderer* renderer, RenderContext& context, Size rangeStart, Size rangeEnd ) override;
	virtual void	PostRender		( IRenderer* renderer, RenderContext& context ) override;

	virtual void	NestedPasses	( std::vector< Ptr< IRenderPass > >& passes ) override;

};

