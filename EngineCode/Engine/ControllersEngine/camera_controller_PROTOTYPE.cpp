#include "..\..\stdafx.h"
#include "camera_controller_PROTOTYPE.h"
#include "..\UI_Engine\InputAbstractionLayer.h"


#include "..\..\memory_leaks.h"


camera_controller_PROTOTYPE::camera_controller_PROTOTYPE( InputAbstractionLayer_base* layer )
	:	Base_input_controller(layer)
{
	move_speed = 1000.0;
	rot_speed = 1.0;
}


camera_controller_PROTOTYPE::~camera_controller_PROTOTYPE()
{
}

/*Funkcja g��wna odpowiedzialna za sterowanie ruchem obiektu.*/
using namespace STANDARD_LAYERS;

void camera_controller_PROTOTYPE::control_object( Dynamic_object* object )
{
	if ( !abstraction_layer->is_active() )
		return;
	//pobieramy tablic� przycisk�w
	const char* button_state = abstraction_layer->get_buttons_table();

	XMVECTOR forward = XMVectorSet( 0.0, 0.0, 0.0, 0.0 );
	XMVECTOR left = XMVectorSet( 0.0, 0.0, 0.0, 0.0 );

	XMVECTOR down_rotate = XMVectorSet( 0.0, 0.0, 0.0, 0.0 );
	XMVECTOR left_rotate = XMVectorSet( 0.0, 0.0, 0.0, 0.0 );

	//ruch do przodu wzd�u� kierunku patrzenia, albo do ty�u
	if ( button_state[PROTOTYPE::FORWARD] && !button_state[PROTOTYPE::BACKWARD] )
	{
		XMVECTOR versor = XMVectorSet( 0.0, 0.0, 1.0, 0.0 );
		XMVECTOR orientation = object->get_orientation( );
		XMMATRIX orient = XMMatrixRotationQuaternion( orientation );
		forward = XMVector4Transform( versor, orient );
		forward *= move_speed;
	}
	else if ( button_state[PROTOTYPE::BACKWARD] && !button_state[PROTOTYPE::FORWARD] )
	{
		XMVECTOR versor = XMVectorSet( 0.0, 0.0, -1.0, 0.0 );
		XMVECTOR orientation = object->get_orientation( );
		XMMATRIX orient = XMMatrixRotationQuaternion( orientation );
		forward = XMVector4Transform( versor, orient );
		forward *= move_speed;
	}

	//ruch w lewo albo w prawo
	if ( button_state[PROTOTYPE::LEFT] && !button_state[PROTOTYPE::RIGHT] )
	{
		XMVECTOR versor = XMVectorSet( -1.0, 0.0, 0.0, 0.0 );
		XMVECTOR orientation = object->get_orientation( );
		XMMATRIX orient = XMMatrixRotationQuaternion( orientation );
		left = XMVector4Transform( versor, orient );
		left *= move_speed;
	}
	else if ( button_state[PROTOTYPE::RIGHT] && !button_state[PROTOTYPE::LEFT] )
	{
		XMVECTOR versor = XMVectorSet( 1.0, 0.0, 0.0, 0.0 );
		XMVECTOR orientation = object->get_orientation( );
		XMMATRIX orient = XMMatrixRotationQuaternion( orientation );
		left = XMVector4Transform( versor, orient );
		left *= move_speed;
	}

	forward = forward + left;		//dodajemy ruch posuwisty
	object->set_speed( forward );	//nadajemy obiektowi pr�dko��, poprzedni� si� nie przejmujemy

	//ruch obrotowy
	//mamy lewoskr�tny uk�ad wsp�rzednych, wi�c wektory s� odwrotnie zwr�cone wzgl�dem normalnych wektor�w
	if ( button_state[PROTOTYPE::DOWN] && !button_state[PROTOTYPE::UP] )
	{
		XMVECTOR versor = XMVectorSet( 1.0, 0.0, 0.0, 0.0 );
		XMVECTOR orientation = object->get_orientation( );
		XMMATRIX orient = XMMatrixRotationQuaternion( orientation );
		down_rotate = XMVector4Transform( versor, orient );
		down_rotate *= rot_speed;
	}
	else if ( button_state[PROTOTYPE::UP] && !button_state[PROTOTYPE::DOWN] )
	{
		XMVECTOR versor = XMVectorSet( -1.0, 0.0, 0.0, 0.0 );
		XMVECTOR orientation = object->get_orientation( );
		XMMATRIX orient = XMMatrixRotationQuaternion( orientation );
		down_rotate = XMVector4Transform( versor, orient );
		down_rotate *= rot_speed;
	}

	if ( button_state[PROTOTYPE::TURN_LEFT] && !button_state[PROTOTYPE::TURN_RIGHT] )
	{
		left_rotate = XMVectorSet( 0.0, -1.0, 0.0, 0.0 );
		//zawsze obracamy wzgl�dem osi pionowej, dlatego nie mno�ymy przez kwaternion orientacji
		left_rotate *= rot_speed;
	}
	else if ( button_state[PROTOTYPE::TURN_RIGHT] && !button_state[PROTOTYPE::TURN_LEFT] )
	{
		left_rotate = XMVectorSet( 0.0, 1.0, 0.0, 0.0 );
		//zawsze obracamy wzgl�dem osi pionowej, dlatego nie mno�ymy przez kwaternion orientacji
		left_rotate *= rot_speed;
	}

	down_rotate = down_rotate + left_rotate;		//obroty si� dodaj� jak wektory
	XMVECTOR length = XMVector3Length( down_rotate );	//d�ugo�� wektora jest jednocze�nie szybko�ci� obrotu
	float f_length = XMVectorGetX( length );
	down_rotate = XMVectorSetW( down_rotate, f_length );	//zapisujemy szybko�� obrotu w sk�adowej w

	object->set_rotation_speed( down_rotate );
}
