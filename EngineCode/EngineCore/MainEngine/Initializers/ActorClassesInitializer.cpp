/**@file ActorClassesInitializer.cpp
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.

@brief Rejestracja domyœlnych klas dla aktorów.*/
#include "EngineCore/stdafx.h"

#include "EngineCore/MainEngine/Engine.h"
#include "EngineCore/MainEngine/EngineContext.h"
#include "EngineCore/Actors/ActorsManager.h"



/**@brief Rejestruje podstawowe klasy aktorów.*/
bool Engine::InitDefaultActorsClasses()
{
	auto actorsFactory = Context->actorsManager->GetActorFactory();
	
	actorsFactory.RegisterClass( GetTypeidName< StaticObject >(), StaticObject::Create );
	actorsFactory.RegisterClass( GetTypeidName< CollisionObject >(), CollisionObject::Create );
	actorsFactory.RegisterClass( GetTypeidName< DynamicObject >(), DynamicObject::Create );
	actorsFactory.RegisterClass( GetTypeidName< PhysicalObject >(), PhysicalObject::Create );
	actorsFactory.RegisterClass( GetTypeidName< DynamicMeshObject >(), DynamicMeshObject::Create );
	actorsFactory.RegisterClass( GetTypeidName< AnimationObject >(), AnimationObject::Create );
	actorsFactory.RegisterClass( GetTypeidName< CameraObject >(), CameraObject::Create );
	actorsFactory.RegisterClass( GetTypeidName< ComplexObject >(), ComplexObject::Create );

	
	return true;
}


