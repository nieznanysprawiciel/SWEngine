#pragma once
/**
@file IMainPass.h
@author nieznanysprawiciel
@copyright File is part of graphic engine SWEngine.
*/


#include "EngineCore/DisplayEngine/RenderPasses/IRenderPass.h"

/**@brief */
class IMainPass : public IRenderPass
{
private:
protected:

	CameraActor*						m_mainCamera;
	ResourcePtr< RenderTargetObject >	m_mainTarget;

public:
	explicit		IMainPass() = default;
					~IMainPass() = default;

	void			SetMainCamera	( CameraActor* camera );
	CameraActor*	GetMainCamera	();

};
