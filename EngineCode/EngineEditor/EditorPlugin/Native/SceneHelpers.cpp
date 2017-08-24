/**
@file SceneHelpers.cpp
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/


#pragma unmanaged
#pragma warning(disable : 4561)	// DirectXMath converting fastcall to stdcall

#include "EngineEditor/EditorPlugin/stdafx.h"

#include "SceneHelpers.h"


#include "EngineCore/MainEngine/MacrosSwitches.h"

#include "EngineCore/Actors/ActorObjects.h"

#include "EngineCore/EngineHelpers/Intersections.h"


#include "EngineEditor/EditorPlugin/EngineInterfaceProvider.h"



namespace sw
{


 // ================================ //
//
void		SceneHelpers::SetActorInitialPosition		( StaticActor* actor, double mouseX, double mouseY )
{
	auto engine = EngineInterfaceProvider::GetEngine();

	auto& camera = engine->GetMainCamera();
	DirectX::XMVECTOR position = DirectX::XMLoadFloat3( &camera.Position );

	DirectX::XMVECTOR rayDir = Intersections::ComputeMouseRayDirection( engine, camera, (float)mouseX, (float)mouseY );
	DirectX::XMVECTOR intersection = Intersections::PlaneXZIntersection( rayDir, position, 0.f );

	actor->Teleport( intersection );
}

}	// sw



