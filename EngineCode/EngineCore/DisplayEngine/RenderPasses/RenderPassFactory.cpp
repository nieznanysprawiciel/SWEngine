/**
@file RenderPassFactory.cpp
@author nieznanysprawiciel
@copyright File is part of graphic engine SWEngine.
*/
#include "EngineCore/stdafx.h"

#include "RenderPassFactory.h"

#include "RenderingLogic/DefaultLogic.h"
#include "Shading/ForwardRendering.h"

#include "EngineCore/ModelsManager/AssetsManager.h"

#include "swCommonLib/Common/MemoryLeaks.h"


AssetsManager*		IRenderPass::s_assetsManager = nullptr;
RenderPassFactory*	IRenderPass::s_renderPassFactory = nullptr;


/**@brief Creates default values for shading passes.*/
RenderPassFactory::RenderPassFactory()
{}


// ================================ //
//
Ptr< IRenderingLogicPass >		RenderPassFactory::CreateDefaultLogic()
{
	return MakePtr< DefaultLogic >();
}

// ================================ //
//
Ptr< IShadingPass >				RenderPassFactory::CreateDefaultShadingPass()
{
	return MakePtr< ForwardRendering >();
}

// ================================ //
//
Ptr< IShadingPass >				RenderPassFactory::CreateDefaultForwardShading()
{
	return MakePtr< ForwardRendering >();
}

// ================================ //
//
Ptr< IShadingPass >				RenderPassFactory::CreateDefaultDeferredShading()
{
	return Ptr< IShadingPass >();
}
