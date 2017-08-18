/**
@file DisplayerInitializer.cpp
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/



#include "EngineCore/stdafx.h"

#include "EngineCore/MainEngine/Engine.h"
#include "EngineCore/MainEngine/EngineContext.h"
#include "EngineCore/DisplayEngine/DisplayEngine.h"
#include "swGraphicAPI/Resources/ResourcesFactory.h"

#include "swCommonLib/Common/MemoryLeaks.h"



namespace sw
{

/**@brief Initializes DisplayEngine.


@return Returns always true.*/
bool		Engine::InitDisplayer()
{
	IRenderer* renderer = m_renderingSystem->GetRenderer();
	
	sw::gui::HostWindow* mainWindow = m_windows[ 0 ];

	Context->displayEngine->InitRenderer( renderer );
	Context->displayEngine->InitDisplayer( Context->modelsManager, mainWindow->GetRenderTarget().Ptr() );

	return true;
}


}	// sw


