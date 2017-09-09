#pragma once
/**
@file CameraActor.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/



#include "DynamicActor.h"
#include "CameraData.h"



namespace sw
{



/**@brief Klasa bazowa dla wszystkich obiektów kamer w silniku.
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

	DirectX::XMFLOAT4X4		m_projectionMatrix;		///<Macierz projekcji. Dla ka¿dej kamery mo¿e byæ inna. @attention Na razie nieu¿ywane. Macierz projekcji jest ustawiana na sta³e w DisplayEngine.

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

	bool				GetIsPerspective	() { return m_isPerspective; }
	float				GetWidth			() { return m_width; }
	float				GetHeight			() { return m_height; }
	float				GetNearPlane		() { return m_nearPlane; }
	float				GetFarPlane			() { return m_farPlane; }
	float				GetFov				() { return m_fov; }

	DirectX::XMFLOAT4X4		GetProjection	() { return m_projectionMatrix; }

	CameraData			GetCameraData		();

	static ActorBase*	Create				() { return new CameraActor; }

private:
	void				UpdateMatrix		();
};


}	// sw