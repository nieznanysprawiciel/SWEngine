#include "..\..\stdafx.h"
#include "..\ModelsManager\meshes_textures_materials.h"

using namespace DirectX;

typedef struct ConstantPerFrame
{
	XMFLOAT4X4		view_matrix;
	XMFLOAT4X4		projection_matrix;
	XMFLOAT3		light_direction[ENGINE_MAX_LIGHTS];
	XMFLOAT3		light_color[ENGINE_MAX_LIGHTS];
	float			time;
	float			time_lag;
} ConstantPerFrame;


typedef struct ConstantPerMesh
{
	XMMATRIX				world_matrix;
	DirectX::XMFLOAT4		Diffuse;		//Sk³adowa przezroczystoœci odnosi siê do ca³ego materia³u
	DirectX::XMFLOAT3		Ambient;
	DirectX::XMFLOAT3		Specular;
	DirectX::XMFLOAT3		Emissive;
	float					Power;
} ConstantPerMesh;
