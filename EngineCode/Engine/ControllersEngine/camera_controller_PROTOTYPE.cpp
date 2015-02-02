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

/*Funkcja g³ówna odpowiedzialna za sterowanie ruchem obiektu.*/
using namespace STANDARD_LAYERS;

void camera_controller_PROTOTYPE::control_object( Dynamic_object* object )
{
	if ( !abstraction_layer->is_active() )
		return;
	//pobieramy tablicê przycisków
	const char* button_state = abstraction_layer->get_buttons_table();

	XMVECTOR forward = XMVectorSet( 0.0, 0.0, 0.0, 0.0 );
	XMVECTOR left = XMVectorSet( 0.0, 0.0, 0.0, 0.0 );

	XMVECTOR down_rotate = XMVectorSet( 0.0, 0.0, 0.0, 0.0 );
	XMVECTOR left_rotate = XMVectorSet( 0.0, 0.0, 0.0, 0.0 );

	//ruch do przodu wzd³u¿ kierunku patrzenia, albo do ty³u
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
	object->set_speed( forward );	//nadajemy obiektowi prêdkoœæ, poprzedni¹ siê nie przejmujemy

	//ruch obrotowy
	//mamy lewoskrêtny uk³ad wspó³rzednych, wiêc wektory s¹ odwrotnie zwrócone wzglêdem normalnych wektorów
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
		//zawsze obracamy wzglêdem osi pionowej, dlatego nie mno¿ymy przez kwaternion orientacji
		left_rotate *= rot_speed;
	}
	else if ( button_state[PROTOTYPE::TURN_RIGHT] && !button_state[PROTOTYPE::TURN_LEFT] )
	{
		left_rotate = XMVectorSet( 0.0, 1.0, 0.0, 0.0 );
		//zawsze obracamy wzglêdem osi pionowej, dlatego nie mno¿ymy przez kwaternion orientacji
		left_rotate *= rot_speed;
	}

	down_rotate = down_rotate + left_rotate;		//obroty siê dodaj¹ jak wektory
	XMVECTOR length = XMVector3Length( down_rotate );	//d³ugoœæ wektora jest jednoczeœnie szybkoœci¹ obrotu
	float f_length = XMVectorGetX( length );
	down_rotate = XMVectorSetW( down_rotate, f_length );	//zapisujemy szybkoœæ obrotu w sk³adowej w

	object->set_rotation_speed( down_rotate );
}
