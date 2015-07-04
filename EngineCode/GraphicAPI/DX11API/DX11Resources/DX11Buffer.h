#pragma once

#include "GraphicAPI\MeshResources.h"
#include "../DX11Initializer/DX11APIObjects.h"


class DX11Buffer	:	public BufferObject, public DX11APIObjects
{
private:
	ID3D11Buffer*			m_buffer;
protected:
	~DX11Buffer();
public:
	DX11Buffer();

	ID3D11Buffer* get() { return m_buffer; }

};

