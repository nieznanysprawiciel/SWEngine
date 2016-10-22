#pragma once
/**
@file IRenderPass.h
@author nieznanysprawiciel
@copyright File is part of graphic engine SWEngine.
*/

#include "Common/EngineObject.h"

#include "GraphicAPI/MeshResources.h"
#include "GraphicAPI/RasterizerState.h"
#include "GraphicAPI/BlendingState.h"
#include "GraphicAPI/DepthStencilState.h"
#include "GraphicAPI/IRenderer.h"

#include "EngineCore/DisplayEngine/RenderContext.h"

#include <DirectXMath.h>
#include <vector>

class RenderTargetObject;
class CameraActor;
class StaticActor;
class ShaderInputLayout;



/**@brief Base class for render passes.*/
class IRenderPass : public EngineObject
{
public:
	enum class ActorAddPolicy
	{
		All,
		None,
		Static,
		Dynamic
	};

private:
protected:

	ActorAddPolicy						m_addPolicy;

	ResourcePtr< BlendingState >		m_blendingState;
	ResourcePtr< RasterizerState >		m_rasterizer;

public:

	void			AddActor	( StaticActor* actor, bool isDynamic );
	void			DeleteActor	( StaticActor* actor );

	virtual void	Render		( IRenderer* renderer, RenderContext& context ) = 0;
};
