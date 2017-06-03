#include "EngineCore/stdafx.h"
/**
@file RenderingHelpers.cpp
@author nieznanysprawiciel
@copyright File is part of graphic engine SWEngine.
*/

#include "RenderingHelpers.h"


using namespace DirectX;


namespace sw
{

// ================================ //
//
CameraConstants			RenderingHelper::CreateCameraData	( CameraActor* camera, float timeInterval, float timeLag )
{
	CameraConstants data;
	data.Time = timeInterval + timeLag * FIXED_MOVE_UPDATE_INTERVAL;

	XMMATRIX viewMatrix;
	XMMATRIX viewProjMatrix;
	if( camera == nullptr )
	{
		viewMatrix = XMMatrixIdentity();	//tymczasowe
		viewProjMatrix = XMMatrixIdentity();	//tymczasowe
	}
	else
	{
		XMVECTOR position = camera->GetInterpolatedPosition( timeLag );
		XMVECTOR orientation = camera->GetInterpolatedOrientation( timeLag );

		XMStoreFloat3( &data.CameraPosition, position );

		inverse_camera_position( position );
		inverse_camera_orientation( orientation );

		viewMatrix = XMMatrixTranslationFromVector( position );
		XMMATRIX rotation_matrix = XMMatrixRotationQuaternion( orientation );
		viewMatrix = viewMatrix * rotation_matrix;

		data.ProjectionMatrix = camera->GetProjection();
		viewProjMatrix = XMMatrixMultiply( viewMatrix, XMLoadFloat4x4( &data.ProjectionMatrix ) );

		viewMatrix = XMMatrixTranspose( viewMatrix );
		viewProjMatrix = XMMatrixTranspose( viewProjMatrix );
	}

	XMStoreFloat4x4( &data.ViewMatrix, viewMatrix );
	XMStoreFloat4x4( &data.ViewProjectionMatrix, viewProjMatrix );

	return data;
}

// ================================ //
//
void					RenderingHelper::UpdateCameraBuffer		( CameraActor* camera, IRenderer* renderer, RenderContext& context )
{
	CameraConstants cameraData = RenderingHelper::CreateCameraData( camera, context.TimeInterval, context.TimeLag );

	UpdateBufferCommand updateCommand;
	updateCommand.FillData = (uint8*)&cameraData;
	updateCommand.Buffer = context.CameraBuffer;
	updateCommand.Size = sizeof( CameraConstants );

	renderer->UpdateBuffer( updateCommand );
}

// ================================ //
//
void					RenderingHelper::UpdateLightBuffer		( IRenderer* renderer, RenderContext& context )
{
	LightConstants lightData = context.LightModule->FillLightsData( context.TimeLag );

	UpdateBufferCommand updateCommand;
	updateCommand.FillData = (uint8*)&lightData;
	updateCommand.Buffer = context.LightBuffer;
	updateCommand.Size = sizeof( LightConstants );

	renderer->UpdateBuffer( updateCommand );
}

// ================================ //
//
void					RenderingHelper::ClearRenderTargetAndDepth	( IRenderer* renderer, RenderTargetObject* target, DirectX::XMFLOAT4 color, float depth )
{
	ClearRenderTargetCommand clearCommand;
	clearCommand.ClearDepth = true;
	clearCommand.ClearRenderTarget = true;
	clearCommand.ClearStencil = false;
	clearCommand.RenderTarget = target;
	clearCommand.DepthValue = depth;
	clearCommand.ClearColor = color;

	renderer->ClearRenderTarget( clearCommand );
}

}	// sw

