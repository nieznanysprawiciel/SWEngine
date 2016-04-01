#pragma once

#include <DirectXMath.h>
#include <vector>

class RenderTargetObject;
class CameraActor;
class DynamicMeshActor;
class ShaderInputLayoutObject;

class RenderPass
{
private:
	CameraActor*				m_camera;
	RenderTargetObject*			m_renderTarget;

	DirectX::XMFLOAT4X4			m_projectionMatrix;

	std::vector<DynamicMeshActor*>		m_meshes;			///< @todo To bêdzie trzeba za³atwiæ przez mapy boolów, a wektor bêdzie jeden na ca³y DisplayEngine.
	ShaderInputLayoutObject*			m_bufferLayout;		///< @todo Jeszcze jedna tymczasowoœæ. Layout jest zwi¹zany z buforem a nie passem.
protected:
public:
	RenderPass();
	~RenderPass();

	inline RenderTargetObject*		GetRenderTarget()		{ return m_renderTarget; }
	inline CameraActor*			GetCamera()				{ return m_camera; }
	inline DirectX::XMMATRIX		GetProjectionMatrix()	{ return DirectX::XMLoadFloat4x4( &m_projectionMatrix ); }
	inline ShaderInputLayoutObject*	GetLayout()				{ return m_bufferLayout; }

	inline std::vector<DynamicMeshActor*>&		GetMeshes()	{ return m_meshes; }

	void		AddMesh				( DynamicMeshActor* );
	void		SetRenderTarget		( RenderTargetObject* );
	void		SetLayout			( ShaderInputLayoutObject* );
};
