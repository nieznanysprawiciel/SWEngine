#include "EditorActorsFactory.h"

#include "EngineEditor/EditorPlugin/EnginePointerProvider.h"
#include "EngineCore/MainEngine/Engine.h"

#include "EngineCore/UIEngine/InputAbstractionLayer.h"
#include "EngineCore/Actors/BasicActors/DynamicMeshActor.h"
#include "EngineCore/ControllersEngine/BasicControllers/Editor/GizmoController.h"
#include "EngineCore/ControllersEngine/BasicControllers/SpectatorCameraController.h"

#include <msclr/marshal_cppstd.h>

namespace EditorPlugin
{

/**@brief */
ActorWrapper^		EditorActorsFactory::CreateGizmoActor( System::String^ meshPath )
{
	auto engine = EnginePointerProvider::GetEngine();

	DynamicMeshActor* actor = static_cast< DynamicMeshActor* >( engine->Actors.CreateActor( rttr::type::get< DynamicMeshActor >() ) );
	engine->Actors.AddToModules( actor, ActorInfoFlag::EnablePreController | ActorInfoFlag::EnablePostController | ActorInfoFlag::EnableDisplay );

	// Ustawienie kontrolera
	auto gizmoController = new GizmoController( engine->Input.GetStandardAbstractionLayer( STANDARD_ABSTRACTION_LAYER::PROTOTYPE_BUTTONS ) );
	actor->SetController( gizmoController );

	// Ustawienie odpowiedniego mesha
	auto gizmoModel = engine->Assets.Models.LoadSync( msclr::interop::marshal_as< std::wstring >( meshPath ) );
	assert( gizmoModel );
	actor->SetModel( gizmoModel );

	// Tworzenie wrappera edytorowego dla aktora
	auto actorInfo = engine->Actors.FindActor( actor );
	assert( actorInfo );
	return gcnew ActorWrapper( actor, &actorInfo->second );
}

/**@brief */
ActorWrapper^		EditorActorsFactory::CreateDefaultCamera( bool setAsCurrent )
{
	auto engine = EnginePointerProvider::GetEngine();

	CameraActor* camera = static_cast< CameraActor* >( engine->Actors.CreateActor( rttr::type::get< CameraActor >() ) );
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
