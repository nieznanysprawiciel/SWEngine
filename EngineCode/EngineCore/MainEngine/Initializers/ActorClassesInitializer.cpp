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
	
	actorsFactory.RegisterClass( GetTypeidName< Object >(), Object::Create );
	actorsFactory.RegisterClass( GetTypeidName< StaticActor >(), StaticActor::Create );
	actorsFactory.RegisterClass( GetTypeidName< CollisionActor >(), CollisionActor::Create );
	actorsFactory.RegisterClass( GetTypeidName< DynamicActor >(), DynamicActor::Create );
	actorsFactory.RegisterClass( GetTypeidName< PhysicalActor >(), PhysicalActor::Create );
	actorsFactory.RegisterClass( GetTypeidName< DynamicMeshActor >(), DynamicMeshActor::Create );
	actorsFactory.RegisterClass( GetTypeidName< AnimationActor >(), AnimationActor::Create );
	actorsFactory.RegisterClass( GetTypeidName< CameraActor >(), CameraActor::Create );
	actorsFactory.RegisterClass( GetTypeidName< ComplexActor >(), ComplexActor::Create );

	
	return true;
}


