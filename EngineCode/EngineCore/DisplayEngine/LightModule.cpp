/**
@file LightModule.cpp
@author nieznanysprawiciel
@copyright File is part of graphic engine SWEngine.
*/

#include "EngineCore/stdafx.h"
#include "LightModule.h"


#include "EngineCore/Actors/BasicActors/DirectionalLight.h"
#include "EngineCore/Actors/BasicActors/SpotLight.h"
#include "EngineCore/Actors/BasicActors/PointLight.h"
#include "EngineCore/EngineHelpers/ActorsCommonFunctions.h"

#include "GraphicAPI/BufferInitData.h"
#include "EngineCore/ModelsManager/AssetsManager.h"

#include "GraphicAPI/IRenderer.h"


const wchar_t LIGHTS_CONSTANTS_BUFFER_NAME[] = L"::DisplayEngine::LightsConstants";


using namespace DirectX;


LightModule::LightModule()
	:	m_modelsManager( nullptr )
{}

// ================================ //
//
void		LightModule::AddLightActor( LightBase* light )
{
	m_lights.push_back( light );
}

// ================================ //
//
void		LightModule::RemoveActor( ActorBase* actor )
{
	ActorsCommonFunctions::RemoveActor( m_lights, static_cast< LightBase* >( actor ) );
}

// ================================ //
//
void		LightModule::RemoveAllActors()
{
	m_lights.clear();
}

// ================================ //
//
void		LightModule::Init( AssetsManager* assetsManager )
{
	assert( assetsManager );
	m_modelsManager = assetsManager;

	ConstantBufferInitData lightsInit;
	lightsInit.Data = nullptr;
	lightsInit.DataType = TypeID::get< LightConstants >();
	lightsInit.NumElements = 1;
	lightsInit.ElementSize = sizeof( LightConstants );
	lightsInit.Usage = ResourceUsage::RESOURCE_USAGE_DEFAULT;

	m_lightBuffer = m_modelsManager->CreateConstantsBuffer( LIGHTS_CONSTANTS_BUFFER_NAME, lightsInit );
	assert( m_lightBuffer );
}

// ================================ //
//
void			LightModule::SetAmbientLight( DirectX::XMFLOAT3& color )
{
	m_ambientColor = color;
}

// ================================ //
//
LightConstants	LightModule::FillLightsData( float timeLag )
{
	int numLights = std::min( ENGINE_MAX_LIGHTS, (int)m_lights.size() );

	LightConstants constants;
	constants.NumLights = numLights;
	constants.AmbientColor = m_ambientColor;

	for( int i = 0; i < numLights; ++i )
	{
		constants.LightData[ i ] = FillLightParams( m_lights[ i ], timeLag );
	}

	return constants;
}

// ================================ //
//
LightParams		LightModule::FillLightParams( LightBase* light, float timeLag )
{
	LightParams params;

	LightType type = light->GetLightType();
	params.Color = light->GetColor();
	params.Intensity = light->GetIntensity();
	params.Type = type;
	XMStoreFloat3( &params.Position, light->GetInterpolatedPosition( timeLag ) );

	// Compute light direction. When no rotation is applied, direction vector is [0.0, 0.0, 1.0].
	// This value is not used in case of point light.
	auto lightOrient = light->GetInterpolatedOrientation( timeLag );
	XMVECTOR defaultOrient = DefaultDirectionVec();
	XMVECTOR direction = XMVector3Rotate( defaultOrient, lightOrient );

	XMStoreFloat3( &params.Direction, direction );


	switch( type )
	{
		case LightType::PointLight:
		{	
			PointLight* pointLight = static_cast< PointLight* >( light );
			params.ClampRadius = pointLight->GetClampRadius();
			params.Intensity = pointLight->GetIntensity();
			params.ConstAttenuation = pointLight->GetConstantAttenuation();
			params.LinearAttenuation = pointLight->GetLinearAttenuation();
			params.QuadraticAttenuation = pointLight->GetQuadraticAttenuation();
		}
		case LightType::SpotLight:
		{
			SpotLight* spotLight = static_cast< SpotLight* >( light );
			params.SpotAngle = spotLight->GetSpotAngle();
		}
	}

	return params;
}

/**@brief Updates lights buffer.

Buffer is bound to pixel shader stage at LightsBufferBindingPoint index.*/
BufferObject*	LightModule::UpdateLightsBuffer( IRenderer* renderer, float timeLag )
{
	LightConstants constants = FillLightsData( timeLag );
	renderer->UpdateSubresource( m_lightBuffer.Ptr(), (void*)&constants );

	renderer->PSSetConstantBuffers( LightsBufferBindingPoint, m_lightBuffer.Ptr() );

	return m_lightBuffer.Ptr();
}

/**@brief Default direction of light.

Uninitialized light points in this direction. To change light direction, user rotates
LightActor. Default rotation vector is then rotated by orientation quaternion.*/
DirectX::XMFLOAT3	LightModule::DefaultDirection()
{
	return DirectX::XMFLOAT3( 0.0, 0.0, 1.0 );
}

/**@brief Function differs from LightModule::DefaultDirection only by return type.
@see DefaultDirection*/
DirectX::XMVECTOR LightModule::DefaultDirectionVec()
{
	return XMVectorSet( 0.0, 0.0, 1.0, 0.0 );
}
