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
	XMFLOAT4X4		world_matrix;
	MaterialObject	material;
} ConstantPerMesh;
