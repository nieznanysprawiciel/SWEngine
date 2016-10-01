#pragma once
/**
@file LightModule.h
@author nieznanysprawiciel
@copyright File is part of graphic engine SWEngine.
*/


#include "EngineCore/Actors/BasicActors/LightBase.h"
#include "GraphicAPI/MeshResources.h"
#include "EngineCore/DisplayEngine/ConstantBuffersFormat.h"

#include <vector>


/**@defgroup Lights Lights
@brief Lights

@ingroup Rendering*/


class AssetsManager;

/**@brief Engine module for lights.

@ingroup Lights*/
class LightModule
{
private:
	AssetsManager*					m_modelsManager;

	std::vector< LightBase* >		m_lights;			///< Lights list.
	ResourcePtr< BufferObject >		m_lightBuffer;		///< Constant buffer containing lights data for shaders. @todo Maybe in future there should be multiple buffers.

	DirectX::XMFLOAT3				m_ambientColor;		///< Ambient light color.

protected:
public:
	explicit		LightModule();
					~LightModule() = default;


	void			AddLightActor			( LightBase* object );		///< Adds light to module.
	void			RemoveActor				( ActorBase* actor );		///< Removes light from module.
	void			RemoveAllActors			();							///< Removes all actors from modules.

	void			Init					( AssetsManager* assetsManager );

	void			SetAmbientLight			( DirectX::XMFLOAT3& color );

	LightConstants	FillLightsData			( float timeLag );
	LightParams		FillLightParams			( LightBase* light, float timeLag );
	BufferObject*	UpdateLightsBuffer		( IRenderer* renderer, float timeLag );


public:
	static DirectX::XMFLOAT3		DefaultDirection	();
	static DirectX::XMVECTOR		DefaultDirectionVec	();
};
