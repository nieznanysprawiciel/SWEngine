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
#include "IncludeWindows.h"


#include "EngineEditor/EditorPlugin/Actors/ActorClassMetaInfo.h"
#include "swInputLibrary/InputWPF/WPFInputProxy.h"

#include "Actors/ActorWrapper.h"
#include "Actors/Resources/ResourceWrapper.h"


namespace sw
{


class EngineInterface;
ref class ActorClassMetaInfo;
ref class ActorWrapper;


namespace EditorPlugin
{
using namespace System::Collections::Generic;
using namespace System::Collections::ObjectModel;

/**@brief Wrapper class for engine in managed code.
@ingroup EditorPlugin*/
public ref class EngineWrapper
{
private:

	EngineInterface*			m_engine;
	input::WPFInputProxy*		m_inputWPF;		///< Modu� wej�cia u�ywany domy�lnie przez edytor.
	input::IInput*				m_directInput;	///< Modu� u�ywany w trybie grania w osobnym oknie.

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
	void				ClearActors					();
	void				DeleteActor					( ActorWrapper^ actor );

	List< ActorClassMetaInfo^ >^						CreateActorsMetadata		();
	ObservableCollection< ActorWrapper^ >^				CreateActorsList			();

	ObservableCollection< ResourceWrapper^ >^			CreateTexturesList			();
	ObservableCollection< ResourceWrapper^ >^			CreateShadersList			();
	ObservableCollection< ResourceWrapper^ >^			CreateLayoutsList			();
	ObservableCollection< ResourceWrapper^ >^			CreateBuffersList			();
	ObservableCollection< ResourceWrapper^ >^			CreateRenderTargetsList		();
	ObservableCollection< ResourceWrapper^ >^			CreateMaterialsList			();
	ObservableCollection< ResourceWrapper^ >^			CreateMeshesList			();

	// Input
	void				KeyboardChange				( System::Windows::Input::Key keyId, bool pressed );
	void				MouseButtonChange			( System::Windows::Input::MouseButton button, bool pressed );
	void				MousePositionChange			( double X, double Y );
	void				MouseWheelChange			( double delta );

public:
	EngineWrapper();
};

}
}	// sw

