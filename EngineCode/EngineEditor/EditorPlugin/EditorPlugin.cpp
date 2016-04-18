// EditorPlugin.cpp : Defines the exported functions for the DLL application.
//

#include "EditorPlugin.h"



//#pragma managed(push, off)
//========================================//
// Engine include
#include "EngineCore/MainEngine/Engine.h"
#include "Common/MacrosSwitches.h"


Engine*		EnginePointerProvider::engine = nullptr;

//extern 	HINSTANCE moduleHandle;


namespace EditorPlugin
{

//========================================//
// Engine variables
const int window_width = 700;
const int window_height = 1300;


/**@brief Tworzy obiekt silnika i inicjuje go.
@return Zwraca true, je�eli inicjowanie powid�o si�.*/
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

/**@brief Wywo�uje funkcje odpowiedzialne za przeliczanie po�o�enia obiekt�w.*/
void EngineWrapper::UpdateScene()
{
	float lag = FIXED_MOVE_UPDATE_INTERVAL;
	m_engine->UpdateScene( lag, 0 );
}


/**@brief Renderuje scen�.

Przed wywo�aniem sceny nale�y wywo�a� funkcj� EngineWrapper::UpdateScene,
�eby zaktualizowa� po�o�enia obiekt�w.
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

/**@brief Wywo�uje funkcje wczytuj�ce obiekty testowe silnika.*/
void EngineWrapper::TestScene()
{
	m_engine->test();
}

/**@brief Ustawia domy�ln� kamer� oraz niebo.*/
void EngineWrapper::BasicScene()
{
	m_engine->SetSkydomeAndCamera();
}

/**@brief W��cza/Wy��cza przychwytywanie inputu.*/
void EngineWrapper::EnableInput( bool val )
{
	m_engine->EnableInput( val );
}

/**@brief Twqorzy list� typ�w zarejestrowanych aktor�w.*/
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


List< ActorWrapper^ >^				EngineWrapper::CreateActorsList			()
{
	List< ActorWrapper^ >^ actorsList = gcnew List< ActorWrapper^ >();
	auto& existingActors = m_engine->Actors.GetAllActors();

	for each( auto& actor in existingActors )
	{
		ActorWrapper^ newActor = gcnew ActorWrapper( actor.first, &actor.second );
		actorsList->Add( newActor );
	}

	return actorsList;
}

} //EditorPlugin


//#pragma managed(pop)

