#pragma warning(disable : 4561)	// DirectXMath converting fastcall to stdcall

#include "EngineEditor/EditorPlugin/stdafx.h"

#include "EngineEditor/PropertyWrapperRTTR/Build/IncludeRTTR.h"

#include "ActorWrapper.h"

#include "EngineCore/MainEngine/EngineInterface.h"
#include "EngineCore/Actors/ActorObjects.h"
#include "swCommonLib/Common/EngineObject.h"

#include "EngineCore/Actors/BasicActors/StaticActor.h"
#include "EngineEditor/EditorPlugin/EngineInterfaceProvider.h"

#include <msclr/marshal_cppstd.h>



namespace sw {
namespace EditorPlugin
{

// ================================ //
//
ActorWrapper::ActorWrapper( EngineObject* actor, const ActorInfo* actorInfo )
	: EngineObjectWrapper( actor )
	, m_actorInfo( actorInfo->GetRawInfo() )
{
	m_name = gcnew System::String( "" );
}

/**@brief */
ActorBase*			ActorWrapper::Ptr()
{
	return static_cast< ActorBase* >( GetPtr() );
}


/**@brief Wczytuje mesha z podanej œcie¿ki i przypisuje aktorowi.*/
bool				ActorWrapper::LoadMesh	( System::String^ meshPath )
{
	auto engine = EngineInterfaceProvider::GetEngine();

	auto model = engine->Assets.Meshes.LoadSync( msclr::interop::marshal_as< std::wstring >( meshPath ) );
	auto meshActor = rttr::rttr_cast<StaticActor*>( m_actorPtr );

	return meshActor->SetModel( model );
}

}	// EditorPlugin
}	// sw

