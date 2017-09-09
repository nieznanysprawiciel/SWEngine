// EditorPlugin.cpp : Defines the exported functions for the DLL application.
//
#pragma warning(disable : 4561)	// DirectXMath converting fastcall to stdcall


#include "EngineEditor/EditorPlugin/stdafx.h"

#include "EditorPlugin.h"


//========================================//
// Engine include

#include "EngineEditor/PropertyWrapperRTTR/Build/IncludeRTTR.h"

#include "EngineCore/MainEngine/MacrosSwitches.h"
#include "EngineCore/Actors/ActorObjects.h"

#include "EngineCore/ControllersEngine/BaseClasses/IController.h"
#include "EngineCore/ControllersEngine/BasicControllers/Editor/GizmoController.h"

#include "EngineEditor/EditorPlugin/Native/SceneHelpers.h"

#include "EngineInterfaceProvider.h"
#include "EngineCore/UserApi/Editor/EditorApi.h"

#include <msclr/marshal_cppstd.h>


namespace sw
{


EngineInterface*	EngineInterfaceProvider::engine = nullptr;

//extern 	HINSTANCE moduleHandle;


namespace EditorPlugin
{

//========================================//
// Engine variables
const int window_width = 700;
const int window_height = 1300;


// ================================ //
//
EngineWrapper::EngineWrapper()
	: m_engine( nullptr )
	, m_inputWPF( nullptr )
	, m_directInput( nullptr )
{}


/**@brief Tworzy obiekt silnika i inicjuje go.
@return Zwraca true, je¿eli inicjowanie powid³o siê.*/
bool			EngineWrapper::InitializeEngine( System::IntPtr moduleHandle )
{

	//if( 0 == GetModuleHandleEx( GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, NULL, &moduleHandle ) )
	//	return false;
	HINSTANCE handle = (HINSTANCE)moduleHandle.ToPointer();
	m_engine = new Engine( handle );
	int result = m_engine->InitEngine( window_width, window_height, false, SW_HIDE );
	if( !result )
	{
		EngineInterfaceProvider::engine = nullptr;
		delete m_engine;
		return false;
	}

	EngineInterfaceProvider::engine = m_engine;

	// Zmieniamy modu³ wejœcia (klawiatury) na proxy WPFowe
	input::WPFInputProxy* proxyInput = new input::WPFInputProxy();

	input::InputInitInfo initInfo;
	initInfo.AppInstance = handle;
	initInfo.WndHandle = nullptr;

	bool result2 = proxyInput->Init( initInfo );
	assert( result2 );

	m_directInput = Api::EditorApi::ChangeInputModule( proxyInput );
	m_inputWPF = proxyInput;

	return result2;
}

/**@brief Zwalnia zasoby silnika.*/
void			EngineWrapper::ReleaseEngine()
{
	EngineInterfaceProvider::engine = nullptr;
	Api::EditorApi::ReleaseEngine();
}

// ================================ //
//
System::IntPtr	EngineWrapper::GetRenderTarget( System::UInt16 width, System::UInt16 height )
{
	return System::IntPtr( Api::EditorApi::GetRenderTargetHandle( width, height ) );
}

/**@brief Wywo³uje funkcje odpowiedzialne za przeliczanie po³o¿enia obiektów.*/
void			EngineWrapper::UpdateScene()
{
	float lag = FIXED_MOVE_UPDATE_INTERVAL;
	Api::EditorApi::UpdateScene( lag, 0 );

	// Poniewa¿ nie dostajemy eventów kiedy kó³ko myszy siê nie rusza, to trzeba zerowaæ za ka¿dym razem.
	// Poza tym trzeba wyczyœciæ eventy wciœniêcia i puszczenia przycisków.
	m_inputWPF->PostUpdate();
}


/**@brief Renderuje scenê.

Przed wywo³aniem sceny nale¿y wywo³aæ funkcjê EngineWrapper::UpdateScene,
¿eby zaktualizowaæ po³o¿enia obiektów.
*/
void			EngineWrapper::RenderScene()
{
	float lag = FIXED_MOVE_UPDATE_INTERVAL;
	Api::EditorApi::RenderScene( lag, 0 );
}

/**@brief Pokazuje okno aplikacji stworzone w EngineCore.*/
void			EngineWrapper::ShowWindow()
{
	m_engine->ShowAppWindow( SW_SHOW );
}

/**@brief Wywo³uje funkcje wczytuj¹ce obiekty testowe silnika.*/
void			EngineWrapper::TestScene()
{
	m_engine->test();
}

/**@brief Ustawia domyœln¹ kamerê oraz niebo.*/
void			EngineWrapper::BasicScene()
{
	m_engine->SetSkydomeAndCamera();
}

/**@brief W³¹cza/Wy³¹cza przychwytywanie inputu.*/
void			EngineWrapper::EnableInput			( bool val )
{
	Api::EditorApi::EnableInput( val );
}


/**@brief Tworzy aktora.*/
ActorWrapper^						EngineWrapper::CreateActor				( System::String^ actorName )
{
	auto actor = m_engine->Actors.CreateActor( msclr::interop::marshal_as< std::string >( actorName ) );
	auto actorData = m_engine->Actors.FindActor( actor );

	if( TypeID::get( *actor ).is_derived_from< CameraActor >() )
		m_engine->Actors.AddToModules( actor, ActorInfoFlag::AsCamera );
	else if( TypeID::get( *actor ).is_derived_from< LightBase >() )
		m_engine->Actors.AddToModules( actor, ActorInfoFlag::AsLight );
	else if( TypeID::get( *actor ).is_derived_from< StaticActor >() )
		m_engine->Actors.AddToModules( actor, ActorInfoFlag::EnableDisplay );

	if( actor && actorData )
		return gcnew ActorWrapper( (EngineObject*)actor, &actorData->second );

	return nullptr;
}


/**@brief Tworzy aktora i ustawia go na pozycji wskazywanej przez wspó³rzêdne myszy.

Wspó³rzêdne s¹ przeliczane na wektor zgodnie z aktualnie ustawion¹ kamer¹.
Wyliczane jest przeciêcie prostej na której le¿y wektor z p³aszczyzn¹ XZ (Y = 0 przynajmniej na razie).
Aktor jest ustawiany w tym punkcie.*/
ActorWrapper^						EngineWrapper::CreateActor				( System::String^ actorName, double mouseX, double mouseY )
{
	ActorWrapper^ newActor = CreateActor( actorName );
	if( newActor == nullptr )
		return nullptr;

	StaticActor* actor = static_cast<StaticActor*>( newActor->GetActorPtr().ToPointer() );
	SceneHelpers::SetActorInitialPosition( actor, mouseX, mouseY );

	return newActor;
}

/**@brief Przyczepia do wybranego aktora (selection) kontroler gizma.*/
void								EngineWrapper::SelectActor				( ActorWrapper^ gizmo, ActorWrapper^ selection )
{
	auto gizmoPtr = static_cast< DynamicActor* >( gizmo->GetActorPtr().ToPointer() );
	auto selectionPtr = static_cast< StaticActor* >( selection->GetActorPtr().ToPointer() );

	assert( rttr::rttr_cast<DynamicActor*>( gizmoPtr ) );

	auto gizmoController = static_cast< editor::GizmoController* >( gizmoPtr->GetController() );
	assert( rttr::rttr_cast< editor::GizmoController* >( gizmoController ) );

	gizmoController->SetFollowedActor( selectionPtr );
}

/**@brief Usuwa wszystkich aktorów i czyœci wszystkie ustawienia silnika.*/
void EngineWrapper::ClearActors()
{
	m_engine->Actors.DeleteAllActors();
}

/**@brief */
void EngineWrapper::DeleteActor( ActorWrapper^ actor )
{
	m_engine->Actors.DeleteActor( actor->Ptr() );
}

/**@brief Tworzy listê typów zarejestrowanych aktorów.*/
List< ActorClassMetaInfo^ >^		EngineWrapper::CreateActorsMetadata		()
{
	auto& registeredClasses = m_engine->Actors.GetRegisteredClasses();
	List< ActorClassMetaInfo^ >^ actorsList = gcnew List< ActorClassMetaInfo^ >();

	for( auto& regClass : registeredClasses )
	{
		ActorClassMetaInfo^ actorInfo = gcnew ActorClassMetaInfo( regClass.second.get_raw_type() );
		actorsList->Add( actorInfo );
	}

	return actorsList;
}

/**@brief */
ObservableCollection< ActorWrapper^ >^		EngineWrapper::CreateActorsList			()
{
	ObservableCollection< ActorWrapper^ >^ actorsList = gcnew ObservableCollection< ActorWrapper^ >();
	auto& existingActors = m_engine->Actors.GetAllActors();

	for each( auto& actor in existingActors )
	{
		ActorWrapper^ newActor = gcnew ActorWrapper( (EngineObject*)actor.first, &actor.second );
		actorsList->Add( newActor );
	}

	auto& actorNamesMap = m_engine->Actors.GetActorsNames();
	for each( auto& nameActorPair in actorNamesMap )
	{
		for each ( auto actor in actorsList )
		{
			if( actor->Ptr() == nameActorPair.second )
			{
				actor->ActorName = gcnew System::String( nameActorPair.first.c_str() );
				break;
			}
		}
	}

	return actorsList;
}

//====================================================================================//
//			Resources list	
//====================================================================================//

template< typename CollectionType >
ObservableCollection< ResourceWrapper^ >^			CopyResourceList	( CollectionType& resourcesList )
{
	ObservableCollection< ResourceWrapper^ >^ resourcesCollection = gcnew ObservableCollection< ResourceWrapper^ >();

	for each( auto& resource in resourcesList )
	{
		ResourceWrapper^ newResource = gcnew ResourceWrapper( resource.Ptr() );
		resourcesCollection->Add( newResource );
	}

	return resourcesCollection;
}

template< typename CollectionType >
void												AppendResourceList	( CollectionType& resourcesList, ObservableCollection< ResourceWrapper^ >^ resourcesCollection )
{
	for each( auto& resource in resourcesList )
	{
		ResourceWrapper^ newResource = gcnew ResourceWrapper( resource.Ptr() );
		resourcesCollection->Add( newResource );
	}
}

/**@brief */
ObservableCollection< ResourceWrapper^ >^			EngineWrapper::CreateTexturesList()
{
	auto resourcesList = m_engine->Assets.Textures.List();
	return CopyResourceList( resourcesList );
}

/**@brief */
ObservableCollection< ResourceWrapper^ >^			EngineWrapper::CreateMaterialsList()
{
	auto resourcesList = m_engine->Assets.Materials.List();
	return CopyResourceList( resourcesList );
}

/**@brief */
ObservableCollection< ResourceWrapper^ >^			EngineWrapper::CreateMeshesList()
{
	auto resourcesList = m_engine->Assets.Meshes.List();
	return CopyResourceList( resourcesList );
}

/**@brief */
ObservableCollection< ResourceWrapper^ >^			EngineWrapper::CreateShadersList()
{
	auto resourcesList1 = m_engine->Assets.Shaders.ListVertexShaders();
	auto collection = CopyResourceList( resourcesList1 );

	auto resourcesList2 = m_engine->Assets.Shaders.ListPixelShaders();
	AppendResourceList( resourcesList2, collection );

	return collection;
}

/**@brief */
ObservableCollection< ResourceWrapper^ >^			EngineWrapper::CreateRenderTargetsList()
{
	auto resourcesList = m_engine->Assets.RenderTargets.List();
	return CopyResourceList( resourcesList );
}

/**@brief */
ObservableCollection< ResourceWrapper^ >^			EngineWrapper::CreateLayoutsList()
{
	auto resourcesList = m_engine->Assets.Shaders.ListShaderLayouts();
	return CopyResourceList( resourcesList );
}

/**@brief */
ObservableCollection< ResourceWrapper^ >^			EngineWrapper::CreateBuffersList()
{
	auto resourcesList1 = m_engine->Assets.Buffers.ListVertexBuffers();
	auto collection = CopyResourceList( resourcesList1 );

	auto resourcesList2 = m_engine->Assets.Buffers.ListIndexBuffers();
	AppendResourceList( resourcesList2, collection );

	auto resourcesList3 = m_engine->Assets.Buffers.ListConstantBuffers();
	AppendResourceList( resourcesList3, collection );

	return collection;
}

//====================================================================================//
//			Input	
//====================================================================================//

/**@brief */
void		EngineWrapper::KeyboardChange		( System::Windows::Input::Key keyId, bool pressed )
{
	m_inputWPF->KeyboardChange( (int)keyId, pressed );
}

/**@brief */
void		EngineWrapper::MouseButtonChange	( System::Windows::Input::MouseButton button, bool pressed )
{
	m_inputWPF->MouseButtonChange( (int)button, pressed );
}

/**@brief */
void		EngineWrapper::MousePositionChange	( double X, double Y )
{
	m_inputWPF->MousePositionChange( X, Y );
}

/**@brief */
void		EngineWrapper::MouseWheelChange( double delta )
{
	m_inputWPF->MouseWheelChange( delta );
}

} //EditorPlugin


//#pragma managed(pop)

}	// sw
