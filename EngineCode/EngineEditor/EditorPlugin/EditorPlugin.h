#pragma once
/**@file Engine.h
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.

@brief Wrapper dla obiektu silnika do komunikacji z edytorem.
*/

/**
@defgroup EngineEditor
@ingroup ModulesStructure
@brief Edytor oraz potrzebne narzêdzia.
*/


/**
@defgroup EditorPlugin
@ingroup EngineEditor

@brief Wrapper na kod silnika w @ref EngineCore.

Plugin zawiera funkcje, nadaj¹ce siê do wywo³ania z kodu zarz¹dzanego (managed code) edytora
napisanego w C#.
*/



//========================================//
// windows include
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "EngineEditor/EditorPlugin/Actors/ActorClassMetaInfo.h"
#include "EngineEditor/EditorPlugin/EnginePointerProvider.h"
#include "EngineCore/UIEngine/InputLibrary/InputWPF/WPFInputProxy.h"

#include "Actors/ActorWrapper.h"

class Engine;
ref class ActorClassMetaInfo;
ref class ActorWrapper;


namespace EditorPlugin
{
using namespace System::Collections::Generic;
using namespace System::Collections::ObjectModel;

	/**@brief Klasa wrapera do inicjowania silnika i renderowania po stronie kodu zarz¹dzanego.
	@ingroup EditorPlugin*/
	public ref class EngineWrapper
	{
	private:
		Engine*				m_engine;
		WPFInputProxy*		m_inputWPF;		///< Modu³ wejœcia u¿ywany domyœlnie przez edytor.
		IInput*				m_directInput;	///< Modu³ u¿ywany w trybie grania w osobnym oknie.

	protected:
	public:
		bool				InitializeEngine			( System::IntPtr moduleHandle );
		void				ReleaseEngine				();
		System::IntPtr		GetRenderTarget				( System::UInt16 width, System::UInt16 height );

		void				UpdateScene					();
		void				RenderScene					();

		void				ShowWindow					();

		void				TestScene					();
		void				BasicScene					();
		void				EnableInput					( bool val );

		// Actors
		ActorWrapper^		CreateActor					( System::String^ actorName, double mouseX, double mouseY );
		ActorWrapper^		CreateActor					( System::String^ actorName );
		void				SelectActor					( ActorWrapper^ gizmo, ActorWrapper^ selection );

		List< ActorClassMetaInfo^ >^						CreateActorsMetadata		();
		ObservableCollection< ActorWrapper^ >^				CreateActorsList			();

		// Input
		void				KeyboardChange				( System::Windows::Input::Key keyId, bool pressed );
		void				MouseButtonChange			( System::Windows::Input::MouseButton button, bool pressed );
		void				MousePositionChange			( double X, double Y );
		void				MouseWheelChange			( double delta );

	public:
		EngineWrapper();
	};

}