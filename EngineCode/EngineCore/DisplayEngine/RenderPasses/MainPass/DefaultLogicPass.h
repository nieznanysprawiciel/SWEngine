#pragma once
/**
@file DefaultLogicPass.h
@author nieznanysprawiciel
@copyright File is part of graphic engine SWEngine.
*/

#include "EngineCore/DisplayEngine/RenderPasses/IMainPass.h"



/**@brief */
class DefaultLogicPass : public IMainPass
{
private:
protected:
public:
	DefaultLogicPass() = default;
	~DefaultLogicPass() = default;


	// Inherited via IMainPass
	virtual void		Render		( IRenderer* renderer, RenderContext& context ) override;

};
