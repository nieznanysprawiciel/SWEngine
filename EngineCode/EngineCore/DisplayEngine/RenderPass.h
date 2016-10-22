#pragma once

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





class RenderPassDepracated
{
private:
	CameraActor*				m_camera;
	RenderTargetObject*			m_renderTarget;

	DirectX::XMFLOAT4X4			m_projectionMatrix;

	std::vector< StaticActor* >	m_meshes;			///< @todo To bêdzie trzeba za³atwiæ przez mapy boolów, a wektor bêdzie jeden na ca³y DisplayEngine.
	ShaderInputLayout*			m_bufferLayout;		///< @todo Jeszcze jedna tymczasowoœæ. Layout jest zwi¹zany z buforem a nie passem.
protected:
public:
	RenderPassDepracated();
	~RenderPassDepracated();

	inline RenderTargetObject*		GetRenderTarget()		{ return m_renderTarget; }
	inline CameraActor*			GetCamera()				{ return m_camera; }
	inline DirectX::XMMATRIX		GetProjectionMatrix()	{ return DirectX::XMLoadFloat4x4( &m_projectionMatrix ); }
	inline ShaderInputLayout*	GetLayout()				{ return m_bufferLayout; }

	inline std::vector< StaticActor* >&		GetMeshes()	{ return m_meshes; }

	void		AddMesh				( StaticActor* );
	void		SetRenderTarget		( RenderTargetObject* );
	void		SetLayout			( ShaderInputLayout* );
};

