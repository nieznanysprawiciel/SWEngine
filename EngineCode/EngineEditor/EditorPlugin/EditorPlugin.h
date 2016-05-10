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
		ActorWrapper^		CreateActor					( System::String^ actorName, int mouseX, int mouseY );
		
		List< ActorClassMetaInfo^ >^						CreateActorsMetadata		();
		ObservableCollection< ActorWrapper^ >^				CreateActorsList			();


	};

}