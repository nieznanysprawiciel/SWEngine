#pragma once

#include "StaticActor.h"


class IController;

/**@brief Klasa bazowa dla obiekt�w dynamicznych.

Klasa zawiera nast�puj�ce komponenty obs�uguj�ce:
- ruch post�powy i obrotowy
- fizyk�
- kontrolery
- podobiekty (@todo napisa� obs�ug� podobiekt�w)

Je�eli jest zdefiniowana sta�a _QUATERNION_SPEED, to pr�dko�ci k�towe s� wyra�one
kwaternionem w przeciwnym razie jest to wektor, w kt�rym sk�adowa w jest k�tem obrotu.
W docelowej wersji b�dzie najprawdopodobniej wybrana opcja z wetorem a nie kwaternionem.

@note Niezaleznie od tego jak jest wyra�ona pr�dko��, orientacja zawsze jest kwaternionem.
@todo Zastanowi� si� czy controller nie powinien by� unique_ptrem.

@ingroup Actors*/
class DynamicActor : public StaticActor
{
	RTTR_REGISTRATION_FRIEND
	RTTR_ENABLE( StaticActor )
protected:

	DirectX::XMFLOAT3		m_speed;				///< Pr�dko�� postepowa obiektu.
	DirectX::XMFLOAT4		m_rotationSpeed;		///< Pr�dko�� k�towa obiektu (wyra�ona wektorem i k�tem obrotu w sk�adowej w).
	IController*			m_controller;			///< Wska�nik na kontroler, poruszaj�cy obiektem.

	float					m_mass;

public:

	explicit			DynamicActor			();	///< Kontruktor ustawi�j�cy zerow� pr�dko�� k�tow� i post�pow�.
	explicit			DynamicActor			( const DirectX::XMFLOAT3& move_speed, const DirectX::XMFLOAT4& rot_speed );	///< Kontruktor ustawia podan� w parametrach pr�dko��.
	virtual				~DynamicActor();

	void				SetSpeed				( const DirectX::XMVECTOR& vector );		///<Ustawia pr�dko�� obiektu @param[in] vector Wektor pr�dko�ci.
	void				SetRotationSpeed		( const DirectX::XMVECTOR& quaternion );	///<Ustawia pr�dko�� obrotow� @param[in] quaternion Wektor pr�dko�ci.
	void				SetRotationSpeed		( const DirectX::XMFLOAT4 axis_angle );		///<Ustawia pr�dko�� obrotow� @param[in] quaternion Wektor pr�dko�ci.
	DirectX::XMVECTOR	GetSpeed				() const;									///< Zwraca pr�dko�� post�pow� obiektu.
	DirectX::XMVECTOR	GetRotationSpeed		() const;									///< Zwraca pr�dko�� obrotow� obiektu.

	void				SetController			( IController* ctrl );						///< Ustawia podany w parametrze kontroler
	IController*		GetController			();

	void				Move					( float time_interval );
	virtual void		MoveComplex				( float time_interval, const DirectX::XMFLOAT3& parent_speed, const DirectX::XMFLOAT4& parent_rotation );

	static ActorBase*	Create()	{ return new DynamicActor; }
};



//====================================================================================//
//			Implementation	
//====================================================================================//

// ================================ //
//
inline void			DynamicActor::SetSpeed			( const DirectX::XMVECTOR& vector )
{ 
	XMStoreFloat3( &m_speed, vector );
}

// ================================ //
//
inline void			DynamicActor::SetRotationSpeed	( const DirectX::XMVECTOR& quaternion )
{ 
	XMStoreFloat4( &m_rotationSpeed, quaternion );
}

// ================================ //
//
inline void			DynamicActor::SetRotationSpeed	( const DirectX::XMFLOAT4 axisAngle )
{ 
	m_rotationSpeed = axisAngle;
}

// ================================ //
//
inline DirectX::XMVECTOR	DynamicActor::GetSpeed	() const 
{ 
	return XMLoadFloat3( &m_speed ); 
}

// ================================ //
//
inline DirectX::XMVECTOR	DynamicActor::GetRotationSpeed	() const
{
	return XMLoadFloat4( &m_rotationSpeed );
}



