#pragma unmanaged


#include "SceneHelpers.h"


#include "Common/MacrosSwitches.h"

#include "EngineCore/MainEngine/Engine.h"
#include "EngineCore/Actors/ActorObjects.h"

#include "EngineCore/EngineHelpers/Intersections.h"


#include "EngineEditor/EditorPlugin/EnginePointerProvider.h"


void		SceneHelpers::SetActorInitialPosition		( StaticActor* actor, double mouseX, double mouseY )
{
	auto engine = EnginePointerProvider::GetEngine();

	auto& camera = engine->GetMainCamera();
	DirectX::XMVECTOR position = DirectX::XMLoadFloat3( &camera.Position );

	DirectX::XMVECTOR rayDir = Intersections::ComputeMouseRayDirection( engine, camera, (float)mouseX, (float)mouseY );
	DirectX::XMVECTOR intersection = Intersections::PlaneXZIntersection( rayDir, position, 0.f );

	actor->Teleport( intersection );
}




