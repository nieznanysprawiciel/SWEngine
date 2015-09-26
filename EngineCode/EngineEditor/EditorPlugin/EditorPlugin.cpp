// EditorPlugin.cpp : Defines the exported functions for the DLL application.
//

#include "EditorPlugin.h"
#include "Common/macros_switches.h"

namespace EditorPlugin
{

//========================================//
// Engine variables
const int window_width = 700;
const int window_height = 1300;

/**@brief Tworzy obiekt silnika i inicjuje go.
@return Zwraca true, je¿eli inicjowanie powid³o siê.*/
bool EngineWrapper::InitializeEngine()
{
	m_engine = new Engine( GetModuleHandle(NULL) );
	int result = m_engine->InitEngine( window_width, window_height, false, SW_HIDE );
	if( !result )
	{
		delete m_engine;
		return false;
	}

	return true;
}

/**@brief Zwalnia zasoby silnika.*/
void EngineWrapper::ReleaseEngine()
{
	delete m_engine;
}

System::IntPtr EngineWrapper::GetRenderTarget()
{
	return System::IntPtr( m_engine->GetRenderTargetHandle() );
}

void EngineWrapper::UpdateScene()
{
	float lag = FIXED_MOVE_UPDATE_INTERVAL;
	m_engine->UpdateScene( lag, 0 );
}

void EngineWrapper::RenderScene()
{
	float lag = FIXED_MOVE_UPDATE_INTERVAL;
	m_engine->RenderScene( lag, 0 );
}

void EngineWrapper::ShowWindow()
{
	m_engine->ShowAppWindow( SW_SHOW );
}

void EngineWrapper::TestScene()
{
	m_engine->test();
}

} //EditorPlugin
