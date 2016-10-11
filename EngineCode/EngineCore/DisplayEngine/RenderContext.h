#pragma once
/**
@file RenderContext.h
@author nieznanysprawiciel
@copyright File is part of graphic engine SWEngine.
*/

class DisplayEngine;
class LightModule;

/**@brief Context which will be pass to RenderPass::Render function.*/
struct RenderContext
{
	DisplayEngine*		DisplayEngine;
	LightModule*		LightModule;
	// Culling
};

