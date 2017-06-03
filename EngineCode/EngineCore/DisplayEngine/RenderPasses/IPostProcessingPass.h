#pragma once
/**
@file IPostProcessingPass.h
@author nieznanysprawiciel
@copyright File is part of graphic engine SWEngine.
*/

#include "EngineCore/DisplayEngine/RenderPasses/IRenderPass.h"


namespace sw
{

/**@brief Interface for post processing passes.

@ingroup RenderPasses*/
class IPostprocessingPass : public IRenderPass
{
private:
protected:

	ResourcePtr< RenderTargetObject >		m_inputTarget;
	ResourcePtr< RenderTargetObject >		m_outputTarget;

public:
	IPostprocessingPass() = default;
	~IPostprocessingPass() = default;


	virtual void		SetInputTarget		( const ResourcePtr< RenderTargetObject >& input )		= 0;
	virtual void		SetOutputTarget		( const ResourcePtr< RenderTargetObject >& output )		= 0;

	virtual ResourcePtr< RenderTargetObject >		GetOutputTarget		()							= 0;
	virtual ResourcePtr< RenderTargetObject >		GetInputTarget		()							= 0;
};

}	// sw
