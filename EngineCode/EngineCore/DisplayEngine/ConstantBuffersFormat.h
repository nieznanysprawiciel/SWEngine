#pragma once
/**
@file ConstantBuffersFormat.h
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.

@brief Plik zawiera deklaracje buforów sta³ych dla vertex i piksel shadera.*/


#include "DirectXMath.h"


namespace sw
{


typedef uint32 Padding4Bytes;
typedef uint64 Padding8Bytes;
typedef uint16 Padding2Bytes;
typedef uint8 Padding1Bytes;


/** \brief Bufor sta³ych dla shadera zmieniaj¹cy siê raz na klatkê.

@deprecated Nadchodzi wszystko nowe!

Bufory sta³ych dla shaderów musz¹ mieæ wielkoœæ bêd¹c¹ wielokrotnoœci¹ 16.
Wszystkie dane w GPU s¹ przechowywane w postaci wektorów 4-wymiarowych.
Dlatego lepiej nie wysy³aæ danych o innej liczbie wymiarów, bo na pewno coœ
siê zepsuje.

Ewentualnie je¿eli chcemy usprawniæ dostêp do danych po strnie CPU i podawaæ
wektory 3-wymiarowe, to trzeba uzupe³niæ czwarta wspó³rzêdn¹ jakimœ pustym floatem.*/
typedef struct ConstantPerFrame
{
	DirectX::XMFLOAT4X4		view_matrix;
	DirectX::XMFLOAT4X4		projection_matrix;
	DirectX::XMFLOAT4		light_direction[ ENGINE_MAX_LIGHTS ];		// Wspó³rzêdne s¹ zanegowane, ¿eby shader mia³ mniej roboty
	DirectX::XMFLOAT4		light_color[ ENGINE_MAX_LIGHTS ];
	DirectX::XMFLOAT4		ambient_light;
	DirectX::XMFLOAT4		time;				///< Czas. Wartoœæ jest powielona do wszystkich komponentów.
	DirectX::XMFLOAT4		time_lag;			///< Czas wzglêdem ostatniej klatki. Wartoœæ jest powielona do wszystkich komponentów.

	ConstantPerFrame()
	{
		DirectX::XMStoreFloat4x4( &view_matrix, DirectX::XMMatrixIdentity() );
		DirectX::XMStoreFloat4x4( &projection_matrix, DirectX::XMMatrixIdentity() );
		light_direction[ 0 ] = DirectX::XMFLOAT4( 0, 0, 0, 0 );
		light_direction[ 1 ] = DirectX::XMFLOAT4( 0, 0, 0, 0 );
		light_color[ 0 ] = DirectX::XMFLOAT4( 0, 0, 0, 0 );
		light_color[ 1 ] = DirectX::XMFLOAT4( 0, 0, 0, 0 );
		ambient_light = DirectX::XMFLOAT4( 0, 0, 0, 0 );
	}

} ConstantPerFrame;


/** \brief Bufor sta³ych dla shadera zmieniaj¹cy siê dla ka¿dej czêœci mesha.

@deprecated Nadchodzi wszystko nowe!
@deprecated Temporarily used as material buffer.

Obiekt zawiera macierz przekszta³cenia œwiata dla danej czêœci obiektu oraz dane o materia³ach.
Materia³y s¹ wektorami 4-wymiarowymi tylko ze wzglêdu na to, ¿e takie s¹ rejestry na GPU.
U¿ywane s¹ tylko pierwsze 3 elementy wektora. Jedynie w kanale Diffuse ostatnia wspó³rzêdna odpowiada za przezroczystoœæ.
*/
typedef struct ConstantPerMesh
{
	DirectX::XMFLOAT4		Diffuse;		//Sk³adowa przezroczystoœci odnosi siê do ca³ego materia³u
	DirectX::XMFLOAT4		Ambient;
	DirectX::XMFLOAT4		Specular;
	DirectX::XMFLOAT3		Emissive;
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


/**@brief Binding point index for camera buffer.
@see CameraConstants
@ingroup ConstantBuffers*/
const uint8	CameraBufferBindingPoint = 0;


/**@brief Binding point index for transform buffer.
@see CameraConstants
@ingroup ConstantBuffers*/
const uint8 TransformBufferBindingPoint = 1;

/**@brief Binding point index for lights buffer.

@note This is for pixel shader only.

@see CameraConstants
@ingroup ConstantBuffers*/
const uint8 LightsBufferBindingPoint = 1;

/**@brief Binding point index for material buffer.
@see CameraConstants
@ingroup ConstantBuffers*/
const uint8 MaterialBufferBindingPoint = 2;





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
	DirectX::XMFLOAT4		MeshScale;				///< Mesh scaling. One scale value is replicated to 3 components. Other scaling is not supported. @todo Consider applying scale to WorldMatrix.
};


/**@brief Description of light parameters.
@ingroup Lights*/
struct LightParams
{
	DirectX::XMFLOAT3		Color;
	float					ClampRadius;
	DirectX::XMFLOAT3		Position;
	float					SpotAngle;
	DirectX::XMFLOAT3		Direction;		// Wspó³rzêdne s¹ zanegowane, ¿eby shader mia³ mniej roboty
	LightType				Type;
	float					ConstAttenuation;
	float					LinearAttenuation;
	float					QuadraticAttenuation;
	float					Intensity;
};


/**@brief Constant buffer layout with light data.

@see LightParams
@ingroup ConstantBuffers
@ingroup Lights*/
struct LightConstants
{
	DirectX::XMFLOAT3		AmbientColor;
	uint32					NumLights;			///< Number of lights. Must be lower then ENGINE_MAX_LIGHTS.
	LightParams				LightData[ ENGINE_MAX_LIGHTS ];
};


}	// sw