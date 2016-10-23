#pragma once
/**
@file IRenderingLogicPass.h
@author nieznanysprawiciel
@copyright File is part of graphic engine SWEngine.
*/


#include "EngineCore/DisplayEngine/RenderPasses/IRenderPass.h"

/**@brief */
class IRenderingLogicPass : public IRenderPass
{
private:
protected:

	CameraActor*						m_mainCamera;
	ResourcePtr< RenderTargetObject >	m_mainTarget;

public:
	explicit		IRenderingLogicPass() = default;
					~IRenderingLogicPass() = default;

	void			SetMainCamera	( CameraActor* camera );
	CameraActor*	GetMainCamera	();

};
