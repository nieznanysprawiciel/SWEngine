/**
@file LightsApi.cpp
@author nieznanysprawiciel
@copyright File is part of graphic engine SWEngine.
*/
#include "EngineCore/stdafx.h"
#include "LightsApi.h"

#include "EngineCore/Actors/BasicActors/DirectionalLight.h"
#include "EngineCore/Actors/BasicActors/SpotLight.h"
#include "EngineCore/Actors/BasicActors/PointLight.h"


#include "EngineCore/MainEngine/EngineContext.h"



// ================================ //
//
void		Api::LightsApi::AddLight		( LightBase* light )
{
	auto lightsModule = Context->displayEngine->GetLightModule();
	lightsModule->AddLightActor( light );
}

// ================================ //
//
void		Api::LightsApi::RemoveLight		( ActorBase* light )
{
	auto lightsModule = Context->displayEngine->GetLightModule();
	lightsModule->RemoveActor( light );
}
