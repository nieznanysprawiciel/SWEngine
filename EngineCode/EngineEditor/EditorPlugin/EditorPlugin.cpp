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

/**@brief Wywo³uje funkcje odpowiedzialne za przeliczanie po³o¿enia obiektów.*/
void EngineWrapper::UpdateScene()
{
	float lag = FIXED_MOVE_UPDATE_INTERVAL;
	m_engine->UpdateScene( lag, 0 );
}


/**@brief Renderuje scenê.

Przed wywo³aniem sceny nale¿y wywo³aæ funkcjê EngineWrapper::UpdateScene,
¿eby zaktualizowaæ po³o¿enia obiektów.
*/
void EngineWrapper::RenderScene()
{
	float lag = FIXED_MOVE_UPDATE_INTERVAL;
	m_engine->RenderScene( lag, 0 );
}

/**@brief Pokazuje okno aplikacji stworzone w EngineCore.*/
void EngineWrapper::ShowWindow()
{
	m_engine->ShowAppWindow( SW_SHOW );
}

/**@brief Wywo³uje funkcje wczytuj¹ce obiekty testowe silnika.*/
void EngineWrapper::TestScene()
{
	m_engine->test();
}

} //EditorPlugin
