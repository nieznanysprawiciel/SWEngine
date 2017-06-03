#pragma once
/**
@file RenderPassFactory.h
@author nieznanysprawiciel
@copyright File is part of graphic engine SWEngine.
*/


#include "IRenderPass.h"
#include "IShadingPass.h"
#include "IShadingPass.h"
#include "IPostProcessingPass.h"
#include "IRenderingLogicPass.h"


namespace sw
{

/**@brief */
class RenderPassFactory
{
private:
protected:
public:
	explicit		RenderPassFactory	();
	~RenderPassFactory	() = default;


	Ptr< IRenderingLogicPass >		CreateDefaultLogic			();
	Ptr< IShadingPass >				CreateDefaultShadingPass	();

	Ptr< IShadingPass >				CreateDefaultForwardShading	();
	Ptr< IShadingPass >				CreateDefaultDeferredShading();
};



}	// sw

