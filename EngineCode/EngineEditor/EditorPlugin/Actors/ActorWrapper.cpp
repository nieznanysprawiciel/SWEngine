#pragma warning(disable : 4561)	// DirectXMath converting fastcall to stdcall
#include "ActorWrapper.h"
#include "Common/RTTR.h"

#include "EngineCore/MainEngine/EngineInterface.h"
#include "EngineCore/Actors/ActorObjects.h"
#include "Common/EngineObject.h"

#include "EngineCore/MainEngine/Engine.h"
#include "EngineCore/Actors/BasicActors/StaticActor.h"
#include "EngineEditor/EditorPlugin/EnginePointerProvider.h"

#include <msclr/marshal_cppstd.h>



namespace EditorPlugin
{


ActorWrapper::ActorWrapper( EngineObject* actor, const ActorInfo* actorInfo )
	:	EngineObjectWrapper( actor )
	,	m_actorInfo( actorInfo->GetRawInfo() )
{
	m_name = gcnew System::String( "" );
}

/**@brief */
ActorBase* ActorWrapper::Ptr()
{
	return static_cast< ActorBase* >( m_actorPtr );
}


/**@brief Wczytuje mesha z podanej œcie¿ki i przypisuje aktorowi.*/
bool				ActorWrapper::LoadMesh	( System::String^ meshPath )
{
	auto engine = EnginePointerProvider::GetEngine();

	auto model = engine->Assets.Meshes.LoadSync( msclr::interop::marshal_as< std::wstring >( meshPath ) );
	auto meshActor = rttr::rttr_cast< StaticActor* >( m_actorPtr );

	return meshActor->SetModel( model );
}

}	// EditorPlugin