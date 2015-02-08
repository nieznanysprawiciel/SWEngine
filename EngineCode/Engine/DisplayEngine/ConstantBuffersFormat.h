#include "..\..\stdafx.h"
#include "..\ModelsManager\meshes_textures_materials.h"

using namespace DirectX;

/* Bufory sta³ych dla shaderów musz¹ miec wielkoœæ bêd¹c¹ wielokrotnoœci¹ 16.
Nie trzeba o to dbaæ w tym przypadku, bo funkcja tworz¹ca bufory alokuje
odrobinê wiêksze.
Ale skoro i tak ma siê pamiêæ i przepustowoœæ marnowaæ to mo¿na spokojnie dope³niæ
jakimiœ informacjami do tych 16.*/

typedef struct ConstantPerFrame
{
	XMFLOAT4X4		view_matrix;
	XMFLOAT4X4		projection_matrix;
	XMFLOAT4		light_direction[ENGINE_MAX_LIGHTS];		// Wspó³rzêdne s¹ zanegowane, ¿eby shader mia³ mniej roboty
	XMFLOAT4		light_color[ENGINE_MAX_LIGHTS];
	XMFLOAT4		ambient_light;
	float			time;
	float			time_lag;
} ConstantPerFrame;


typedef struct ConstantPerMesh
{
	XMMATRIX				world_matrix;
	DirectX::XMFLOAT4		Diffuse;		//Sk³adowa przezroczystoœci odnosi siê do ca³ego materia³u
	DirectX::XMFLOAT4		Ambient;
	DirectX::XMFLOAT4		Specular;
	DirectX::XMFLOAT4		Emissive;
	float					Power;
} ConstantPerMesh;
