#pragma once

#include <DirectXMath.h>
#include <vector>

class RenderTargetObject;
class CameraObject;
class DynamicMeshObject;
class ShaderInputLayoutObject;

class RenderPass
{
private:
	CameraObject*				m_camera;
	RenderTargetObject*			m_renderTarget;

	DirectX::XMFLOAT4X4			m_projectionMatrix;

	std::vector<DynamicMeshObject*>		m_meshes;			///< @todo To bêdzie trzeba za³atwiæ przez mapy boolów, a wektor bêdzie jeden na ca³y DisplayEngine.
	ShaderInputLayoutObject*			m_bufferLayout;		///< @todo Jeszcze jedna tymczasowoœæ. Layout jest zwi¹zany z buforem a nie passem.
protected:
public:
	RenderPass();
	~RenderPass();

	inline RenderTargetObject*		GetRenderTarget()		{ return m_renderTarget; }
	inline CameraObject*			GetCamera()				{ return m_camera; }
	inline DirectX::XMMATRIX		GetProjectionMatrix()	{ return DirectX::XMLoadFloat4x4( &m_projectionMatrix ); }
	inline ShaderInputLayoutObject*	GetLayout()				{ return m_bufferLayout; }

	inline std::vector<DynamicMeshObject*>&		GetMeshes()	{ return m_meshes; }

	void		AddMesh				( DynamicMeshObject* );
	void		SetRenderTarget		( RenderTargetObject* );
	void		SetLayout			( ShaderInputLayoutObject* );
};
