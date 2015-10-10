#pragma once

#include <DirectXMath.h>


class RenderTargetObject;
class CameraObject;


class RenderPass
{
private:
	CameraObject*				m_camera;
	RenderTargetObject*			m_renderTarget;

	DirectX::XMFLOAT4X4			m_projectionMatrix;
protected:
public:
	RenderPass() = default;
	~RenderPass() = default;

};
