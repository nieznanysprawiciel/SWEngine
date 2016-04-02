/**@file ActorClassesInitializer.cpp
@author nieznanysprawiciel
@copyright Plik jest cz�ci� silnika graficznego SWEngine.

@brief Rejestracja domy�lnych klas dla aktor�w.*/
#include "EngineCore/stdafx.h"

#include "EngineCore/MainEngine/Engine.h"
#include "EngineCore/MainEngine/EngineContext.h"
#include "EngineCore/Actors/ActorsManager.h"



/**@brief Rejestruje podstawowe klasy aktor�w.*/
bool Engine::InitDefaultActorsClasses()
{
	auto& actorsFactory = Context->actorsManager->GetActorFactory();
	
	actorsFactory.RegisterClass< Object >( GetTypeidName< Object >(), Object::Create );
	actorsFactory.RegisterClass< StaticActor >( GetTypeidName< StaticActor >(), StaticActor::Create );
	actorsFactory.RegisterClass< CollisionActor >( GetTypeidName< CollisionActor >(), CollisionActor::Create );
	actorsFactory.RegisterClass< DynamicActor >( GetTypeidName< DynamicActor >(), DynamicActor::Create );
	actorsFactory.RegisterClass< PhysicalActor >( GetTypeidName< PhysicalActor >(), PhysicalActor::Create );
	actorsFactory.RegisterClass< DynamicMeshActor >( GetTypeidName< DynamicMeshActor >(), DynamicMeshActor::Create );
	actorsFactory.RegisterClass< AnimationActor >( GetTypeidName< AnimationActor >(), AnimationActor::Create );
	actorsFactory.RegisterClass< CameraActor >( GetTypeidName< CameraActor >(), CameraActor::Create );
	actorsFactory.RegisterClass< ComplexActor >( GetTypeidName< ComplexActor >(), ComplexActor::Create );

	
	return true;
}


