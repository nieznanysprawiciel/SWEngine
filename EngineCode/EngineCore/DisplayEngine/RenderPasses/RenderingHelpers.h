#pragma once
/**
@file RenderingHelpers.h
@author nieznanysprawiciel
@copyright File is part of graphic engine SWEngine.
*/

#include "EngineCore/DisplayEngine/ConstantBuffersFormat.h"
#include "EngineCore/Actors/ActorObjects.h"


#include <DirectXMath.h>

class IRenderer;
struct RenderContext;


/**@brief Rendering helper functions for @ref DisplayEngine and render passes.*/
struct RenderingHelper
{
public:

	/**@brief Creates camera buffer filled with data.*/
	static CameraConstants		CreateCameraData			( CameraActor* camera, float timeInterval, float timeLag );

	/**@brief Fills camera buffer with data and updates.*/
	static void					UpdateCameraBuffer			( CameraActor* camera, IRenderer* renderer, RenderContext& context );

	/**@brief Updates light buffer with data from LightModule.*/
	static void					UpdateLightBuffer			( IRenderer* renderer, RenderContext& context );

	/**@brief Default clear command.*/
	static void					ClearRenderTargetAndDepth	( IRenderer* renderer, RenderTargetObject* target, DirectX::XMFLOAT4 color, float depth );

	/**@brief Updates buffer with data.*/
	template< typename BufferDataType >
	static inline void			UpdateBuffer				( IRenderer* renderer, BufferObject* buffer, BufferDataType& data );
};



// ================================ //
//
template< typename BufferDataType >
inline void			RenderingHelper::UpdateBuffer			( IRenderer* renderer, BufferObject* buffer, BufferDataType& data )
{
	UpdateBufferCommand updateCommand;
	updateCommand.Buffer = buffer;
	updateCommand.FillData = (uint8*)&data;
	updateCommand.Size = sizeof( BufferDataType );

	renderer->UpdateBuffer( updateCommand );
}
