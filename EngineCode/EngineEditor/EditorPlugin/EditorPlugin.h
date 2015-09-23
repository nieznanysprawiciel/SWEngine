#pragma once
/**@file Engine.h
@author nieznanysprawiciel
@copyright Plik jest cz�ci� silnika graficznego SWEngine.

@brief Wrapper dla obiektu silnika do komunikacji z edytorem.
*/

/**
@defgroup EngineEditor
@ingroup ModulesStructure
@brief Edytor oraz potrzebne narz�dzia.
*/


/**
@defgroup EditorPlugin
@ingroup EngineEditor

@brief Wrapper na kod silnika w @ref EngineCore.

Plugin zawiera funkcje, nadaj�ce si� do wywo�ania z kodu zarz�dzanego (managed code) edytora
napisanego w C#.
*/



//========================================//
// windows include
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

//========================================//
// Engine include
#include "EngineCore/MainEngine/Engine.h"

namespace EditorPlugin
{
	public ref class EngineWrapper
	{
	private:
		Engine*				m_engine;
	protected:
	public:
		bool				InitializeEngine();
		void				ReleaseEngine();
		System::IntPtr		GetRenderTarget();

		void				UpdateScene();
		void				RenderScene();

		void				ShowWindow();

		void				TestScene();
	};

}