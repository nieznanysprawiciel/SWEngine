#include "EngineCore/stdafx.h"
#include "camera_controller_PROTOTYPE.h"
#include "EngineCore/UIEngine/InputAbstractionLayer.h"


#include "Common/memory_leaks.h"


camera_controller_PROTOTYPE::camera_controller_PROTOTYPE( InputAbstractionLayer_base* layer )
	:	BaseInputController(layer)
{
	move_speed = 1000.0;
	button_rot_speed = 1.0;
	axis_rot_speed = 120.0;
}


camera_controller_PROTOTYPE::~camera_controller_PROTOTYPE()
{
}

/*Funkcja g��wna odpowiedzialna za sterowanie ruchem obiektu.*/
using namespace STANDARD_LAYERS;

void camera_controller_PROTOTYPE::control_object( DynamicObject* object )
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
	if ( button_state[PROTOTYPE_BUTTONS::FORWARD] && !button_state[PROTOTYPE_BUTTONS::BACKWARD] )
	{
		XMVECTOR versor = XMVectorSet( 0.0, 0.0, 1.0, 0.0 );
		XMVECTOR orientation = object->get_orientation( );
		XMMATRIX orient = XMMatrixRotationQuaternion( orientation );
		forward = XMVector4Transform( versor, orient );
		forward *= move_speed;
	}
	else if ( button_state[PROTOTYPE_BUTTONS::BACKWARD] && !button_state[PROTOTYPE_BUTTONS::FORWARD] )
	{
		XMVECTOR versor = XMVectorSet( 0.0, 0.0, -1.0, 0.0 );
		XMVECTOR orientation = object->get_orientation( );
		XMMATRIX orient = XMMatrixRotationQuaternion( orientation );
		forward = XMVector4Transform( versor, orient );
		forward *= move_speed;
	}

	//ruch w lewo albo w prawo
	if ( button_state[PROTOTYPE_BUTTONS::LEFT] && !button_state[PROTOTYPE_BUTTONS::RIGHT] )
	{
		XMVECTOR versor = XMVectorSet( -1.0, 0.0, 0.0, 0.0 );
		XMVECTOR orientation = object->get_orientation( );
		XMMATRIX orient = XMMatrixRotationQuaternion( orientation );
		left = XMVector4Transform( versor, orient );
		left *= move_speed;
	}
	else if ( button_state[PROTOTYPE_BUTTONS::RIGHT] && !button_state[PROTOTYPE_BUTTONS::LEFT] )
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
	if ( button_state[PROTOTYPE_BUTTONS::DOWN] && !button_state[PROTOTYPE_BUTTONS::UP] )
	{
		XMVECTOR versor = XMVectorSet( 1.0, 0.0, 0.0, 0.0 );
		XMVECTOR orientation = object->get_orientation( );
		XMMATRIX orient = XMMatrixRotationQuaternion( orientation );
		down_rotate = XMVector4Transform( versor, orient );
		down_rotate *= button_rot_speed;
	}
	else if ( button_state[PROTOTYPE_BUTTONS::UP] && !button_state[PROTOTYPE_BUTTONS::DOWN] )
	{
		XMVECTOR versor = XMVectorSet( -1.0, 0.0, 0.0, 0.0 );
		XMVECTOR orientation = object->get_orientation( );
		XMMATRIX orient = XMMatrixRotationQuaternion( orientation );
		down_rotate = XMVector4Transform( versor, orient );
		down_rotate *= button_rot_speed;
	}

	if ( button_state[PROTOTYPE_BUTTONS::TURN_LEFT] && !button_state[PROTOTYPE_BUTTONS::TURN_RIGHT] )
	{
		left_rotate = XMVectorSet( 0.0, -1.0, 0.0, 0.0 );
		//zawsze obracamy wzgl�dem osi pionowej, dlatego nie mno�ymy przez kwaternion orientacji
		left_rotate *= button_rot_speed;
	}
	else if ( button_state[PROTOTYPE_BUTTONS::TURN_RIGHT] && !button_state[PROTOTYPE_BUTTONS::TURN_LEFT] )
	{
		left_rotate = XMVectorSet( 0.0, 1.0, 0.0, 0.0 );
		//zawsze obracamy wzgl�dem osi pionowej, dlatego nie mno�ymy przez kwaternion orientacji
		left_rotate *= button_rot_speed;
	}

	//pobieramy tablic� osi
	const float* axis_state = abstraction_layer->get_axis_table();

	float y_axis = axis_state[PROTOTYPE_AXES::Y_AXIS];
	if (y_axis != 0.0f)
	{
		XMVECTOR versor = XMVectorSet(1.0, 0.0, 0.0, 0.0);
		XMVECTOR orientation = object->get_orientation();
		XMMATRIX orient = XMMatrixRotationQuaternion(orientation);
		XMVECTOR axis_down_rotate = XMVector4Transform(versor, orient);
		axis_down_rotate *= y_axis * axis_rot_speed;

		down_rotate += axis_down_rotate;
	}

	float x_axis = axis_state[PROTOTYPE_AXES::X_AXIS];
	if (x_axis != 0.0f)
	{
		XMVECTOR axis_left_rotate = XMVectorSet(0.0, 1.0, 0.0, 0.0);
		axis_left_rotate *= x_axis * axis_rot_speed;

		left_rotate += axis_left_rotate;
	}

	down_rotate = down_rotate + left_rotate;				//obroty si� dodaj� jak wektory
	XMVECTOR length = XMVector3Length( down_rotate );		//d�ugo�� wektora jest jednocze�nie szybko�ci� obrotu
	float f_length = XMVectorGetX( length );
	down_rotate = XMVectorSetW( down_rotate, f_length );	//zapisujemy szybko�� obrotu w sk�adowej w

	object->set_rotation_speed( down_rotate );

	
}
