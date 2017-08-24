#pragma warning(disable : 4561)	// DirectXMath converting fastcall to stdcall
#include "EngineEditor/EditorPlugin/stdafx.h"

#include "EditorActorsFactory.h"

#include "EngineEditor/EditorPlugin/EngineInterfaceProvider.h"

#include "EngineCore/UIEngine/InputAbstractionLayer.h"
#include "EngineCore/Actors/BasicActors/DynamicActor.h"
#include "EngineCore/ControllersEngine/BasicControllers/Editor/GizmoController.h"
#include "EngineCore/ControllersEngine/BasicControllers/SpectatorCameraController.h"

#include <msclr/marshal_cppstd.h>





namespace sw {
namespace EditorPlugin
{

/**@brief */
GizmoActorWrapper^	EditorActorsFactory::CreateGizmoActor( System::String^ translateMesh, System::String^ rotateMesh, System::String^ scaleMesh )
{
	auto engine = EngineInterfaceProvider::GetEngine();

	DynamicActor* actor = static_cast<DynamicActor*>( engine->Actors.CreateActor( rttr::type::get< DynamicActor >() ) );
	engine->Actors.AddToModules( actor, ActorInfoFlag::EnablePreController | ActorInfoFlag::EnablePostController | ActorInfoFlag::EnableDisplay );

	// Loading meshes
	auto translationGizmoMesh = engine->Assets.Meshes.LoadSync( msclr::interop::marshal_as< std::wstring >( translateMesh ) );
	assert( translationGizmoMesh );

	auto rotationGizmoMesh = engine->Assets.Meshes.LoadSync( msclr::interop::marshal_as< std::wstring >( rotateMesh ) );
	assert( rotationGizmoMesh );

	auto scaleGimzoMesh = engine->Assets.Meshes.LoadSync( msclr::interop::marshal_as< std::wstring >( scaleMesh ) );
	//assert( scaleGimzoMesh );


	// Ustawienie kontrolera
	auto gizmoController = new editor::GizmoController( engine->Input.GetStandardAbstractionLayer( STANDARD_ABSTRACTION_LAYER::PROTOTYPE_BUTTONS ), translationGizmoMesh, rotationGizmoMesh, scaleGimzoMesh );
	actor->SetController( gizmoController );

	// Tworzenie wrappera edytorowego dla aktora
	auto actorInfo = engine->Actors.FindActor( actor );
	assert( actorInfo );
	return gcnew GizmoActorWrapper( actor, &actorInfo->second );
}

/**@brief */
ActorWrapper^		EditorActorsFactory::CreateDefaultCamera( bool setAsCurrent )
{
	auto engine = EngineInterfaceProvider::GetEngine();

	CameraActor* camera = static_cast<CameraActor*>( engine->Actors.CreateActor( rttr::type::get< CameraActor >() ) );
	engine->Actors.AddToModules( camera, ActorInfoFlag::EnableMovement | ActorInfoFlag::EnablePreController | ActorInfoFlag::AsCamera );


	//DirectX::XMVECTOR cameraPos = DirectX::XMVectorSet( 0.0, 0.0, 0.0, 0.0 );
	//camera->Teleport( cameraPos );

	// Przypisujemy kontroler ( dla kontrolerów trzeba zrobiæ jakiœ mechanizm przechowywania i zwalniania)
	SpectatorCameraController* controller = new SpectatorCameraController(
		engine->Input.GetStandardAbstractionLayer( STANDARD_ABSTRACTION_LAYER::PROTOTYPE_BUTTONS ) );
	camera->SetController( controller );

	// Wstawiamy kamerê do odpowiednich modu³ów
	if( setAsCurrent )
		engine->Rendering.SetCurrentCamera( camera );


	// Tworzenie wrappera edytorowego dla aktora
	auto actorInfo = engine->Actors.FindActor( camera );
	assert( actorInfo );
	return gcnew ActorWrapper( camera, &actorInfo->second );
}

}	// EditorPlugin
}	// sw
