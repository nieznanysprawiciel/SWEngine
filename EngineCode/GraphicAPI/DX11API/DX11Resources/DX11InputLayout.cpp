#include "DX11InputLayout.h"

#include "Common/MemoryLeaks.h"


DX11InputLayout::DX11InputLayout( ID3D11InputLayout* layout )
{
	m_vertexLayout = layout;
}


DX11InputLayout::~DX11InputLayout()
{
	if( m_vertexLayout )
		m_vertexLayout->Release();
	m_vertexLayout = nullptr;
}
