#pragma once

#include "DynamicActor.h"


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

	DirectX::XMVECTOR	GetDirection()		{	return DirectX::XMLoadFloat3( &Direction );		}
	DirectX::XMVECTOR	GetUpVector()		{	return DirectX::XMLoadFloat3( &UpVector );		}
	DirectX::XMVECTOR	GetRightVector()	{	return DirectX::XMLoadFloat3( &RightVector );	}
	DirectX::XMVECTOR	GetOrientation()	{	return DirectX::XMLoadFloat4( &OrientationQuat );	}
};


/**@brief Klasa bazowa dla wszystkich obiekt�w kamer w silniku.
*/
class CameraActor : public DynamicActor
{
	friend class DisplayEngine;

	RTTR_ENABLE( DynamicActor )
	RTTR_REGISTRATION_FRIEND
protected:

	float					m_fov;
	float					m_width;
	float					m_height;
	float					m_nearPlane;
	float					m_farPlane;
	bool					m_isPerspective;

	DirectX::XMFLOAT4X4		m_projectionMatrix;		///<Macierz projekcji. Dla ka�dej kamery mo�e by� inna. @attention Na razie nieu�ywane. Macierz projekcji jest ustawiana na sta�e w DisplayEngine.

public:
	explicit CameraActor();

	void				SetPerspectiveProjectionMatrix		( float angle, float width, float height, float nearPlane, float farPlane );
	void				SetOrthogonalProjectionMatrix		( float width, float height, float nearPlane, float farPlane );

	void				SetPerspective		( bool value );
	void				SetWidth			( float width );
	void				SetHeight			( float height );
	void				SetNearPlane		( float plane );
	void				SetFarPlane			( float plane );
	void				SetFov				( float fov );

	bool				GetIsPerspective	()					{ return m_isPerspective; }
	float				GetWidth			()					{ return m_width; }
	float				GetHeight			()					{ return m_height; }
	float				GetNearPlane		()					{ return m_nearPlane; }
	float				GetFarPlane			()					{ return m_farPlane; }
	float				GetFov				()					{ return m_fov; }

	DirectX::XMFLOAT4X4		GetProjection	()					{ return m_projectionMatrix; }

	CameraData			GetCameraData		();

	static ActorBase*	Create				()					{ return new CameraActor; }

private:
	void				UpdateMatrix		();
};

