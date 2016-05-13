/**@file ActorClassesInitializer.cpp
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.

@brief Rejestracja domyœlnych klas dla aktorów.*/
#include "EngineCore/stdafx.h"

#include "EngineCore/MainEngine/Engine.h"
#include "EngineCore/MainEngine/EngineContext.h"
#include "EngineCore/Actors/ActorsManager.h"



RTTR_REGISTRATION
{
    using namespace rttr;

	RTTR_REGISTRATION_STANDARD_TYPE_VARIANTS( DirectX::XMFLOAT2 )
	RTTR_REGISTRATION_STANDARD_TYPE_VARIANTS( DirectX::XMFLOAT3 )
	RTTR_REGISTRATION_STANDARD_TYPE_VARIANTS( DirectX::XMFLOAT4 )

	registration::class_< DirectX::XMFLOAT4 >( "DirectX::XMFLOAT4" )
		.property( "X", &DirectX::XMFLOAT4::x )
		.property( "Y", &DirectX::XMFLOAT4::y )
		.property( "Z", &DirectX::XMFLOAT4::z )
		.property( "W", &DirectX::XMFLOAT4::w );

	registration::class_< DirectX::XMFLOAT3 >( "DirectX::XMFLOAT3" )
		.property( "X", &DirectX::XMFLOAT3::x )
		.property( "Y", &DirectX::XMFLOAT3::y )
		.property( "Z", &DirectX::XMFLOAT3::z );

	registration::class_< DirectX::XMFLOAT2 >( "DirectX::XMFLOAT2" )
		.property( "X", &DirectX::XMFLOAT2::x )
		.property( "Y", &DirectX::XMFLOAT2::y );
}


/**@brief Rejestruje podstawowe klasy aktorów.*/
bool Engine::InitDefaultActorsClasses()
{
	auto& actorsFactory = Context->actorsManager->GetActorFactory();
	
	//actorsFactory.RegisterClass< ActorBase >( GetTypeidName< ActorBase >(), ActorBase::Create );
	//actorsFactory.RegisterClass< StaticActor >( GetTypeidName< StaticActor >(), StaticActor::Create );
	//actorsFactory.RegisterClass< CollisionActor >( GetTypeidName< CollisionActor >(), CollisionActor::Create );
	//actorsFactory.RegisterClass< DynamicActor >( GetTypeidName< DynamicActor >(), DynamicActor::Create );
	//actorsFactory.RegisterClass< PhysicalActor >( GetTypeidName< PhysicalActor >(), PhysicalActor::Create );
	//actorsFactory.RegisterClass< AnimationActor >( GetTypeidName< AnimationActor >(), AnimationActor::Create );
	actorsFactory.RegisterClass< ComplexActor >( GetTypeidName< ComplexActor >(), ComplexActor::Create );
	actorsFactory.RegisterClass< DynamicMeshActor >( GetTypeidName< DynamicMeshActor >(), DynamicMeshActor::Create );
	actorsFactory.RegisterClass< CameraActor >( GetTypeidName< CameraActor >(), CameraActor::Create );
	actorsFactory.RegisterClass< PathNode >( GetTypeidName< PathNode >(), PathNode::Create );
	actorsFactory.RegisterClass< SpawnPoint >( GetTypeidName< SpawnPoint >(), SpawnPoint::Create );
	actorsFactory.RegisterClass< PointLight >( GetTypeidName< PointLight >(), PointLight::Create );
	actorsFactory.RegisterClass< DirectionalLight >( GetTypeidName< DirectionalLight >(), DirectionalLight::Create );
	actorsFactory.RegisterClass< SpotLight >( GetTypeidName< SpotLight >(), SpotLight::Create );
	actorsFactory.RegisterClass< TriggerActor >( GetTypeidName< TriggerActor >(), TriggerActor::Create );


	return true;
}


