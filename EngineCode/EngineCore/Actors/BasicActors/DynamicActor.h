#pragma once

#include "CollisionActor.h"


/**@brief Klasa bazowa dla obiektów dynamicznych.

Je¿eli jest zdefiniowana sta³a _QUATERNION_SPEED, to prêdkoœci k¹towe s¹ wyra¿one
kwaternionem w przeciwnym razie jest to wektor, w którym sk³adowa w jest k¹tem obrotu.
W docelowej wersji bêdzie najprawdopodobniej wybrana opcja z wetorem a nie kwaternionem.

@note Niezaleznie od tego jak jest wyra¿ona prêdkoœæ, orientacja zawsze jest kwaternionem.*/
class DynamicActor : public CollisionActor
{
	RTTR_ENABLE( CollisionActor )
protected:

	DirectX::XMFLOAT3		speed;				///< Prêdkoœæ postepowa obiektu.
	DirectX::XMFLOAT4		rotation_speed;		///< Prêdkoœæ k¹towa obiektu (wyra¿ona wektorem i k¹tem obrotu w sk³adowej w).
	Controller*				controller;			///< WskaŸnik na kontroler, poruszaj¹cy obiektem.

public:

	DynamicActor();	///< Kontruktor ustawi¹j¹cy zerow¹ prêdkoœæ k¹tow¹ i postêpow¹.
	DynamicActor( const DirectX::XMFLOAT3& move_speed, const DirectX::XMFLOAT4& rot_speed );	///< Kontruktor ustawia podan¹ w parametrach prêdkoœæ.

	void				SetSpeed				( const DirectX::XMVECTOR& vector )		{ XMStoreFloat3( &speed, vector ); }	///<Ustawia prêdkoœæ obiektu @param[in] vector Wektor prêdkoœci.
	void				SetRotationSpeed		( const DirectX::XMVECTOR& quaternion )	{ XMStoreFloat4( &rotation_speed, quaternion ); }	///<Ustawia prêdkoœæ obrotow¹ @param[in] quaternion Wektor prêdkoœci.
	void				SetRotationSpeed		( const DirectX::XMFLOAT4 axis_angle )	{ rotation_speed = axis_angle; }		///<Ustawia prêdkoœæ obrotow¹ @param[in] quaternion Wektor prêdkoœci.
	DirectX::XMVECTOR	GetSpeed				() const						{ return XMLoadFloat3( &speed ); }	///< Zwraca prêdkoœæ postêpow¹ obiektu.
	DirectX::XMVECTOR	GetRotationSpeed		() const						{ return XMLoadFloat4( &rotation_speed ); }	///< Zwraca prêdkoœæ obrotow¹ obiektu.

	void				SetController			( Controller* ctrl )			{ controller = ctrl; }	///< Ustawia podany w parametrze kontroler
	Controller*			GetController			()								{ return controller; }

	void				Move					( float time_interval );
	virtual void		MoveComplex				( float time_interval, const DirectX::XMFLOAT3& parent_speed, const DirectX::XMFLOAT4& parent_rotation );

	static EngineObject*		Create()	{ return new DynamicActor; }
};

