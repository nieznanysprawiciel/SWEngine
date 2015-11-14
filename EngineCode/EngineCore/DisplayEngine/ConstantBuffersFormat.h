#pragma once
/**
@file ConstantBuffersFormat.h
@author nieznanysprawiciel
@copyright Plik jest cz�ci� silnika graficznego SWEngine.

@brief Plik zawiera deklaracje bufor�w sta�ych dla vertex i piksel shadera.*/

//#include "EngineCore/stdafx.h"
#include "DirectXMath.h"


/** \brief Bufor sta�ych dla shadera zmieniaj�cy si� raz na klatk�. 

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
