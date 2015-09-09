#include "DX11InputLayoutDescriptor.h"
#include "../DX11Initializer/DX11ConstantsMapper.h"

#include <cstring>

void DX11InputLayoutDescriptor::AddRow( const char* semanticName, ResourceFormat format, unsigned int inputSlot, unsigned int byteOffset, bool perInstance, unsigned int instanceDataStep )
{
	D3D11_INPUT_ELEMENT_DESC inputElement;
	inputElement.SemanticName = semanticName;
	inputElement.Format = DX11ConstantsMapper::Get( format );
	inputElement.InputSlot = inputSlot;
	inputElement.AlignedByteOffset = byteOffset;
	inputElement.InstanceDataStepRate = instanceDataStep;
	if( perInstance )
		inputElement.InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_INSTANCE_DATA;
	else
		inputElement.InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;
	inputElement.SemanticIndex = CountSemantic( semanticName );

	m_inputElement.push_back( inputElement );
}

unsigned int DX11InputLayoutDescriptor::CountSemantic( const char * semanticName )
{
	unsigned int numSemantic = 0;

	for( auto element : m_inputElement )
		if( strcmp( element.SemanticName, semanticName ) == 0 )
			++numSemantic;

	return numSemantic;
}
