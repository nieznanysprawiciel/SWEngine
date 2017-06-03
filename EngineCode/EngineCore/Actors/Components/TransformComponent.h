#pragma once
/**
@file TransformComponent.h
@author nieznanysprawiciel
@copyright File is part of graphic engine SWEngine.
*/


#include <DirectXMath.h>

#include "swCommonLib/Common/RTTR.h"

namespace sw
{


/**@defgroup Components Components
@ingroup Actors
@brief Basic actor components.*/



/**@brief Transformation data.

Orientation is stored as quaternion.

@ingroup Components*/
class TransformComponent
{
	RTTR_REGISTRATION_FRIEND;
private:
	DirectX::XMFLOAT4		m_orientation;			///< Orientation quaternion.
	DirectX::XMFLOAT3		m_position;				///< Position.
	float					m_scale;				///< Scale.

public:
	explicit		TransformComponent	();
	~TransformComponent	();

/**@brief Sets scale components.*/
	inline void			SetScale		( float scale ) { m_scale = scale; };
	/**@brief Gets scale component.*/
	inline float		GetScale		() const { return m_scale; }


	void				SetPositionXM		( DirectX::XMVECTOR position );
	void				SetPosition			( const DirectX::XMFLOAT3& position );
	void				SetOrientationXM	( DirectX::XMVECTOR orientation );
	void				SetOrientation		( const DirectX::XMFLOAT4& orientation );

	const DirectX::XMFLOAT3&		GetPosition			() const;
	const DirectX::XMFLOAT4&		GetOrientation		() const;		///< Gets orientation quaterion.

	DirectX::XMVECTOR				GetPositionXM		() const;		///< Gets position. Returns XMVECTOR.
	DirectX::XMVECTOR				GetOrientationXM	() const;		///< Gets orientation quaterion. Returns XMVECTOR.

	float				GetRotationX() const;
	float				GetRotationY() const;
	float				GetRotationZ() const;

	float				GetYaw		() const;
	float				GetPitch	() const;
	float				GetRoll		() const;

	void				SetYaw		( float angle );
	void				SetPitch	( float angle );
	void				SetRoll		( float angle );
};


}	// sw

