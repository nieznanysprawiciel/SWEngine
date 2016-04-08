#pragma once

#include "CollisionActor.h"


/**@brief Klasa bazowa dla obiekt�w dynamicznych.

Je�eli jest zdefiniowana sta�a _QUATERNION_SPEED, to pr�dko�ci k�towe s� wyra�one
kwaternionem w przeciwnym razie jest to wektor, w kt�rym sk�adowa w jest k�tem obrotu.
W docelowej wersji b�dzie najprawdopodobniej wybrana opcja z wetorem a nie kwaternionem.

@note Niezaleznie od tego jak jest wyra�ona pr�dko��, orientacja zawsze jest kwaternionem.*/
class DynamicActor : public CollisionActor
{
	RTTR_ENABLE( CollisionActor )
protected:

	DirectX::XMFLOAT3		speed;				///< Pr�dko�� postepowa obiektu.
	DirectX::XMFLOAT4		rotation_speed;		///< Pr�dko�� k�towa obiektu (wyra�ona wektorem i k�tem obrotu w sk�adowej w).
	Controller*				controller;			///< Wska�nik na kontroler, poruszaj�cy obiektem.

public:

	DynamicActor();	///< Kontruktor ustawi�j�cy zerow� pr�dko�� k�tow� i post�pow�.
	DynamicActor( const DirectX::XMFLOAT3& move_speed, const DirectX::XMFLOAT4& rot_speed );	///< Kontruktor ustawia podan� w parametrach pr�dko��.

	void				SetSpeed				( const DirectX::XMVECTOR& vector )		{ XMStoreFloat3( &speed, vector ); }	///<Ustawia pr�dko�� obiektu @param[in] vector Wektor pr�dko�ci.
	void				SetRotationSpeed		( const DirectX::XMVECTOR& quaternion )	{ XMStoreFloat4( &rotation_speed, quaternion ); }	///<Ustawia pr�dko�� obrotow� @param[in] quaternion Wektor pr�dko�ci.
	void				SetRotationSpeed		( const DirectX::XMFLOAT4 axis_angle )	{ rotation_speed = axis_angle; }		///<Ustawia pr�dko�� obrotow� @param[in] quaternion Wektor pr�dko�ci.
	DirectX::XMVECTOR	GetSpeed				() const						{ return XMLoadFloat3( &speed ); }	///< Zwraca pr�dko�� post�pow� obiektu.
	DirectX::XMVECTOR	GetRotationSpeed		() const						{ return XMLoadFloat4( &rotation_speed ); }	///< Zwraca pr�dko�� obrotow� obiektu.

	void				SetController			( Controller* ctrl )			{ controller = ctrl; }	///< Ustawia podany w parametrze kontroler
	Controller*			GetController			()								{ return controller; }

	void				Move					( float time_interval );
	virtual void		MoveComplex				( float time_interval, const DirectX::XMFLOAT3& parent_speed, const DirectX::XMFLOAT4& parent_rotation );

	static EngineObject*		Create()	{ return new DynamicActor; }
};

