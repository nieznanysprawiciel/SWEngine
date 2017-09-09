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

Generalnie aktorzy nie powinni si� wzajemnie odpytywa� o swoje dane.
Jest zagro�enie, �e z powodu wykonania wielow�tkowego jaki� aktor odczyta niesp�jne dane.
Celem powstania tej struktury jest przechowywanie stanu kamery zarejestrowanego przed
wywo�aniem kontroler�w.

@todo Prawdopodobnie jaki� mechanizm odpytywania si� wzajemnie aktor�w o swoje dane b�dzie potrzebny.
W momencie jak si� on pojawi trzeba si� pozby� tej struktury i zrobi� to nowym mechanizmem.

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
