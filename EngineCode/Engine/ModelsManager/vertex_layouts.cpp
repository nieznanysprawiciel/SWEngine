
#include "..\..\stdafx.h"
#include "meshes_textures_materials.h"



// Opis służący do stworzenia layoutu zrozumiałego dla shaderów.
// Ten format wierzchołka jest docelowo przeznaczony do przetrzymywania standardowych
// meshy.
D3D11_INPUT_ELEMENT_DESC VertexNormalTexCord1_desc[] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};
unsigned int VertexNormalTexCord1_desc_num_of_elements = ARRAYSIZE( VertexNormalTexCord1_desc );

// Opis służący do stworzenia layoutu zrozumiałego dla shaderów.
// Ten format wierzchołka jest docelowo przeznaczony do przetrzymywania
// elementów GUI
D3D11_INPUT_ELEMENT_DESC VertexTexCord1_desc[] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};
unsigned int VertexTexCord1_desc_num_of_elements = ARRAYSIZE( VertexTexCord1_desc );
