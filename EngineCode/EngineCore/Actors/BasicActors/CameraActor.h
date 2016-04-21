#pragma once

#include "DynamicActor.h"


/**@brief Klasa bazowa dla wszystkich obiekt�w kamer w silniku.
*/
class CameraActor : public DynamicActor
{
	friend class DisplayEngine;

	RTTR_ENABLE( DynamicActor )
protected:

	DirectX::XMFLOAT4X4		projection_matrix;		///<Macierz projekcji. Dla ka�dej kamery mo�e by� inna. @attention Na razie nieu�ywane. Macierz projekcji jest ustawiana na sta�e w DisplayEngine.

public:
	void SetProjectionMatrix( float angle, float X_to_Y, float near_plane, float far_plane );


	static ActorBase*			Create()	{ return new CameraActor; }
};

