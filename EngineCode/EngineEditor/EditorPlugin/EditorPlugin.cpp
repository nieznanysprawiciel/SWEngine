// EditorPlugin.cpp : Defines the exported functions for the DLL application.
//

#include "EditorPlugin.h"

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
	int result = m_engine->InitEngine( window_width, window_height, false, 0 );
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
	return System::IntPtr();
}

void EngineWrapper::UpdateScene()
{
	throw gcnew System::NotImplementedException();
}

void EngineWrapper::RenderScene()
{
	throw gcnew System::NotImplementedException();
}

void EngineWrapper::ShowWindow()
{
	throw gcnew System::NotImplementedException();
}

void EngineWrapper::TestScene()
{
	m_engine->test();
}

} //EditorPlugin
