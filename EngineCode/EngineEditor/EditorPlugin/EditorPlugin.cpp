// EditorPlugin.cpp : Defines the exported functions for the DLL application.
//

#include "EditorPlugin.h"



//#pragma managed(push, off)
//========================================//
// Engine include
#include "EngineCore/MainEngine/Engine.h"
#include "Common/MacrosSwitches.h"
#include "EngineCore/Actors/ActorObjects.h"

#include <msclr/marshal_cppstd.h>


Engine*		EnginePointerProvider::engine = nullptr;

//extern 	HINSTANCE moduleHandle;


namespace EditorPlugin
{

//========================================//
// Engine variables
const int window_width = 700;
const int window_height = 1300;


/**@brief Tworzy obiekt silnika i inicjuje go.
@return Zwraca true, je¿eli inicjowanie powid³o siê.*/
bool EngineWrapper::InitializeEngine( System::IntPtr moduleHandle )
{

	//if( 0 == GetModuleHandleEx( GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, NULL, &moduleHandle ) )
	//	return false;
	HINSTANCE handle = (HINSTANCE)moduleHandle.ToPointer();
	m_engine = new Engine( handle );
	int result = m_engine->InitEngine( window_width, window_height, false, SW_HIDE );
	if( !result )
	{
		EnginePointerProvider::engine = nullptr;
		delete m_engine;
		return false;
	}

	EnginePointerProvider::engine = m_engine;

	return true;
}

/**@brief Zwalnia zasoby silnika.*/
void EngineWrapper::ReleaseEngine()
{
	EnginePointerProvider::engine = nullptr;
	delete m_engine;
}

System::IntPtr EngineWrapper::GetRenderTarget( System::UInt16 width, System::UInt16 height )
{
	return System::IntPtr( m_engine->GetRenderTargetHandle( width, height ) );
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

/**@brief Ustawia domyœln¹ kamerê oraz niebo.*/
void EngineWrapper::BasicScene()
{
	m_engine->SetSkydomeAndCamera();
}

/**@brief W³¹cza/Wy³¹cza przychwytywanie inputu.*/
void EngineWrapper::EnableInput( bool val )
{
	m_engine->EnableInput( val );
}

/**@brief */
ActorWrapper^						EngineWrapper::CreateActor				( System::String^ actorName, int mouseX, int mouseY )
{
	auto actor = m_engine->Actors.CreateActor( msclr::interop::marshal_as< std::string >( actorName ) );
	auto actorData = m_engine->Actors.FindActor( actor );

	m_engine->Actors.AddToModules( actor, ActorInfoFlag::EnableDisplay );

	if( rttr::type::get( *actor ).is_derived_from< CameraActor >() )
		m_engine->Actors.AddToModules( actor, ActorInfoFlag::AsCamera );

	if( actor && actorData )
		return gcnew ActorWrapper( (EngineObject*)actor, &actorData->second );

	return gcnew ActorWrapper( nullptr, nullptr );
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

	return actorsList;
}

} //EditorPlugin


//#pragma managed(pop)

