#pragma once
/**
@file ConstantBuffersFormat.h
@author nieznanysprawiciel
@copyright Plik jest cz�ci� silnika graficznego SWEngine.

@brief Plik zawiera deklaracje bufor�w sta�ych dla vertex i piksel shadera.*/

//#include "EngineCore/stdafx.h"
#include "DirectXMath.h"


/** \brief Bufor sta�ych dla shadera zmieniaj�cy si� raz na klatk�.

@deprecated Nadchodzi wszystko nowe!

Bufory sta�ych dla shader�w musz� mie� wielko�� b�d�c� wielokrotno�ci� 16.
Wszystkie dane w GPU s� przechowywane w postaci wektor�w 4-wymiarowych.
Dlatego lepiej nie wysy�a� danych o innej liczbie wymiar�w, bo na pewno co� 
si� zepsuje.

Ewentualnie je�eli chcemy usprawni� dost�p do danych po strnie CPU i podawa�
wektory 3-wymiarowe, to trzeba uzupe�ni� czwarta wsp�rz�dn� jakim� pustym floatem.*/
typedef struct ConstantPerFrame
{
	DirectX::XMFLOAT4X4		view_matrix;
	DirectX::XMFLOAT4X4		projection_matrix;
	DirectX::XMFLOAT4		light_direction[ENGINE_MAX_LIGHTS];		// Wsp�rz�dne s� zanegowane, �eby shader mia� mniej roboty
	DirectX::XMFLOAT4		light_color[ENGINE_MAX_LIGHTS];
	DirectX::XMFLOAT4		ambient_light;
	DirectX::XMFLOAT4		time;				///< Czas. Warto�� jest powielona do wszystkich komponent�w.
	DirectX::XMFLOAT4		time_lag;			///< Czas wzgl�dem ostatniej klatki. Warto�� jest powielona do wszystkich komponent�w.

	ConstantPerFrame()
	{
		DirectX::XMStoreFloat4x4( &view_matrix, DirectX::XMMatrixIdentity() );
		DirectX::XMStoreFloat4x4( &projection_matrix, DirectX::XMMatrixIdentity() );
		light_direction[0] = DirectX::XMFLOAT4( 0, 0, 0, 0 );
		light_direction[1] = DirectX::XMFLOAT4( 0, 0, 0, 0 );
		light_color[0] = DirectX::XMFLOAT4( 0, 0, 0, 0 );
		light_color[1] = DirectX::XMFLOAT4( 0, 0, 0, 0 );
		ambient_light = DirectX::XMFLOAT4( 0, 0, 0, 0 );
	}

} ConstantPerFrame;


/** \brief Bufor sta�ych dla shadera zmieniaj�cy si� dla ka�dej cz�ci mesha.

@deprecated Nadchodzi wszystko nowe!

Obiekt zawiera macierz przekszta�cenia �wiata dla danej cz�ci obiektu oraz dane o materia�ach.
Materia�y s� wektorami 4-wymiarowymi tylko ze wzgl�du na to, �e takie s� rejestry na GPU.
U�ywane s� tylko pierwsze 3 elementy wektora. Jedynie w kanale Diffuse ostatnia wsp�rz�dna odpowiada za przezroczysto��.
*/
typedef struct ConstantPerMesh
{
	DirectX::XMMATRIX		world_matrix;
	DirectX::XMVECTOR		mesh_scale;
	DirectX::XMFLOAT4		Diffuse;		//Sk�adowa przezroczysto�ci odnosi si� do ca�ego materia�u
	DirectX::XMFLOAT4		Ambient;
	DirectX::XMFLOAT4		Specular;
	DirectX::XMFLOAT4		Emissive;
	float					Power;
} ConstantPerMesh;



/**@defgroup ConstantBuffers Constant Buffers
@brief Constant buffers binding points in pipeline.

Engine by defaults binds it's own buffers to pipeline, moreover user can define his own buffers which
can be bound to remaining binding points.

@section DefaultBuffers Default Buffers

There are four buffers which can be bound to pipeline by default:
- Camera buffer (@ref CameraConstants)
- Transform buffer (@ref TransformConstants)
- Light buffer (@ref LightConstants)
- Material buffer (@ref MaterialAsset and @ref ShadingModelBase)

These buffers use first 3 binding points.

Binding Point		| Vertex Shader			| Pixel Shaders
------------------- | -------------			| -----------------------
0					| Camera constants		| Camera constants
1					| Transform constants	| Light constants
2					|						| Material constants

Remaining binding points can be used by custom buffers.

@section CustomBuffers Custom Buffers

@ingroup Rendering*/


typedef float Padding4Bytes;


/**@brief Constant buffer layout with camera data.

Buffer contains not only camera data but also other rarely changing 

@ingroup ConstantBuffers*/
struct CameraConstants
{
	DirectX::XMFLOAT4X4		ViewMatrix;				///< View matrix.
	DirectX::XMFLOAT4X4		ProjectionMatrix;		///< Projection matrix.
	DirectX::XMFLOAT4X4		ViewProjectionMatrix;	///< Multiplied view-projection matrix.
	DirectX::XMFLOAT3		CameraPosition;			///< Position of camera in world space.

	float					Time;					///< Current animation time.
};


/**@brief Constant buffer layout with actor transformation data.
@ingroup ConstantBuffers*/
struct TransformConstants
{
	DirectX::XMFLOAT4X4		WorldMatrix;			///< Transformation to world space.
	DirectX::XMFLOAT3		MeshScale;				///< Mesh scaling. One scale value is replicated to 3 components. Other scaling is not supported. @todo Consider applying scale to WorldMatrix.
};


/**@brief Description of light parameters.*/
struct LightParams
{
	DirectX::XMFLOAT3		Color;
	float					Radius;
	DirectX::XMFLOAT3		Position;
	float					Angle;
	DirectX::XMFLOAT3		Direction;		// Wsp�rz�dne s� zanegowane, �eby shader mia� mniej roboty
	LightType				Type;
};


/**@brief Constant buffer layout with light data.

@see LightParams
@ingroup ConstantBuffers*/
struct LightConstants
{
	DirectX::XMFLOAT3		AmbientColor;
	int						NumLights;
	LightParams				LightDirection[ ENGINE_MAX_LIGHTS ];
};

