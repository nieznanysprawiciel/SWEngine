#pragma once

#include "GraphicAPI\MeshResources.h"
#include "../DX11Initializer/DX11APIObjects.h"


class DX11InputLayout : public ShaderInputLayout
{
private:
	ID3D11InputLayout*			m_vertexLayout;
public:
	DX11InputLayout( ID3D11InputLayout* layout );
	~DX11InputLayout();
};

