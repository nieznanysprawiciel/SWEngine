#pragma once
/**
@file IShadowPass.h
@author nieznanysprawiciel
@copyright File is part of graphic engine SWEngine.
*/

#include "EngineCore/DisplayEngine/RenderPasses/IRenderPass.h"



namespace sw
{

/**@brief Base class for shadow passes.

@ingroup RenderPasses*/
class IShadowPass : public IRenderPass
{
private:
protected:
public:
	explicit		IShadowPass() = default;
	~IShadowPass() = default;


};


}	// sw

