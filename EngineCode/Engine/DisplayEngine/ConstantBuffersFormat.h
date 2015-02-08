#include "..\..\stdafx.h"
#include "..\ModelsManager\meshes_textures_materials.h"

using namespace DirectX;

/* Bufory sta�ych dla shader�w musz� miec wielko�� b�d�c� wielokrotno�ci� 16.
Nie trzeba o to dba� w tym przypadku, bo funkcja tworz�ca bufory alokuje
odrobin� wi�ksze.
Ale skoro i tak ma si� pami�� i przepustowo�� marnowa� to mo�na spokojnie dope�ni�
jakimi� informacjami do tych 16.*/

typedef struct ConstantPerFrame
{
	XMFLOAT4X4		view_matrix;
	XMFLOAT4X4		projection_matrix;
	XMFLOAT4		light_direction[ENGINE_MAX_LIGHTS];		// Wsp�rz�dne s� zanegowane, �eby shader mia� mniej roboty
	XMFLOAT4		light_color[ENGINE_MAX_LIGHTS];
	XMFLOAT4		ambient_light;
	float			time;
	float			time_lag;
} ConstantPerFrame;


typedef struct ConstantPerMesh
{
	XMMATRIX				world_matrix;
	DirectX::XMFLOAT4		Diffuse;		//Sk�adowa przezroczysto�ci odnosi si� do ca�ego materia�u
	DirectX::XMFLOAT4		Ambient;
	DirectX::XMFLOAT4		Specular;
	DirectX::XMFLOAT4		Emissive;
	float					Power;
} ConstantPerMesh;
