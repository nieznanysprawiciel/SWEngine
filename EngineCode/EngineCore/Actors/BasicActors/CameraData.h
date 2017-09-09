#pragma once
/**
@file CameraData.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/

#include <DirectXMath.h>


namespace sw
{


/**@brief Struktura do wymieniania informacji o danych kamery.

Generalnie aktorzy nie powinni siê wzajemnie odpytywaæ o swoje dane.
Jest zagro¿enie, ¿e z powodu wykonania wielow¹tkowego jakiœ aktor odczyta niespójne dane.
Celem powstania tej struktury jest przechowywanie stanu kamery zarejestrowanego przed
wywo³aniem kontrolerów.

@todo Prawdopodobnie jakiœ mechanizm odpytywania siê wzajemnie aktorów o swoje dane bêdzie potrzebny.
W momencie jak siê on pojawi trzeba siê pozbyæ tej struktury i zrobiæ to nowym mechanizmem.

@ingroup Actors*/
struct CameraData
{
	DirectX::XMFLOAT3	Position;
	DirectX::XMFLOAT3	Direction;
	DirectX::XMFLOAT3	UpVector;
	DirectX::XMFLOAT3	RightVector;
	DirectX::XMFLOAT4	OrientationQuat;
	float				Fov;
	float				Width;
	float				Height;
	float				NearPlane;
	float				FarPlane;
	bool				IsPerspective;

	DirectX::XMVECTOR	GetPosition() { return DirectX::XMLoadFloat3( &Position ); }
	DirectX::XMVECTOR	GetDirection() { return DirectX::XMLoadFloat3( &Direction ); }
	DirectX::XMVECTOR	GetUpVector() { return DirectX::XMLoadFloat3( &UpVector ); }
	DirectX::XMVECTOR	GetRightVector() { return DirectX::XMLoadFloat3( &RightVector ); }
	DirectX::XMVECTOR	GetOrientation() { return DirectX::XMLoadFloat4( &OrientationQuat ); }
};


}	// sw
