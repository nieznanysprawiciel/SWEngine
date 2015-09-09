#pragma once

#include "../DX11Initializer/DX11APIObjects.h"
#include "../../MeshResources.h"

#include <vector>


class DX11InputLayoutDescriptor : public InputLayoutDescriptor
{
private:
	std::vector<D3D11_INPUT_ELEMENT_DESC>		m_inputElement;
public:
	DX11InputLayoutDescriptor() = default;
	~DX11InputLayoutDescriptor() = default;

	size_t							GetNumElements()		{ return m_inputElement.size(); }
	D3D11_INPUT_ELEMENT_DESC*		GetDescriptorPtr()		{ return m_inputElement.data(); }

	virtual void					AddRow( const char* semanticName,
											ResourceFormat format,
											unsigned int inputSlot,
											unsigned int byteOffset,
											bool perInstance,
											unsigned int instanceDataStep );

	unsigned int					CountSemantic( const char* semanticName );
};

