/**
@file SceneHelpers.cpp
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/



#pragma warning(disable : 4561)	// DirectXMath converting fastcall to stdcall
#pragma warning(disable: 4793) // methods are compiled as native (clr warning)



#include "EngineEditor/EditorPlugin/stdafx.h"

#include "SceneHelpers.h"


#include "EngineCore/MainEngine/MacrosSwitches.h"

#include "EngineCore/Actors/ActorObjects.h"

#include "EngineCore/EngineHelpers/Intersections.h"


#include "EngineCore/UserApi/Editor/EditorApi.h"



#pragma unmanaged



namespace sw
{


 // ================================ //
//
void		SceneHelpers::SetActorInitialPosition		( StaticActor* actor, double mouseX, double mouseY )
{
	Api::EditorApi engineApi;

	auto& camera = engineApi.GetMainCamera();
	DirectX::XMVECTOR position = camera.GetPosition();

	DirectX::XMVECTOR rayDir = Intersections::ComputeMouseRayDirection( engineApi.GetEngine(), camera, (float)mouseX, (float)mouseY );
	DirectX::XMVECTOR intersection = Intersections::PlaneXZIntersection( rayDir, position, 0.f );

	actor->Teleport( intersection );
}

}	// sw



