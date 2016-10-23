#pragma once
/**
@file IShadingPass.h
@author nieznanysprawiciel
@copyright File is part of graphic engine SWEngine.
*/

#include "EngineCore/DisplayEngine/RenderPasses/IRenderPass.h"



/**@brief Base class for shading passes.

@ingroup RenderPasses*/
class IShadingPass : public IRenderPass
{
private:
protected:
public:
	explicit		IShadingPass() = default;
					~IShadingPass() = default;



	virtual void			SetMainCamera	( CameraActor* camera )		= 0;
	virtual CameraActor*	GetMainCamera	()							= 0;

	virtual void			SetMainRenderTarget	( const ResourcePtr< RenderTargetObject >& target )	= 0;

	virtual ResourcePtr< RenderTargetObject >		GetMainRenderTarget	() = 0;
};
