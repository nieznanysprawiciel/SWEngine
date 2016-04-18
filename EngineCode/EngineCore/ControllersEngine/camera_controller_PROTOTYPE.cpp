#include "EngineCore/stdafx.h"
#include "camera_controller_PROTOTYPE.h"
#include "EngineCore/UIEngine/InputAbstractionLayer.h"


#include "Common/MemoryLeaks.h"

using namespace DirectX;

camera_controller_PROTOTYPE::camera_controller_PROTOTYPE( InputAbstractionLayerBase* layer )
	:	BaseInputController(layer)
{
	move_speed = 1000.0;
	button_rot_speed = 1.0;
	axis_rot_speed = 120.0;
}


camera_controller_PROTOTYPE::~camera_controller_PROTOTYPE()
{
}

/*Funkcja g³ówna odpowiedzialna za sterowanie ruchem obiektu.*/
using namespace STANDARD_LAYERS;

void camera_controller_PROTOTYPE::ControlObject( DynamicActor* object )
{
	if ( !abstraction_layer->IsActive() )
		return;
	//pobieramy tablicê przycisków
	auto& buttonState = abstraction_layer->GetButtonsTable();

	XMVECTOR forward = XMVectorSet( 0.0, 0.0, 0.0, 0.0 );
	XMVECTOR left = XMVectorSet( 0.0, 0.0, 0.0, 0.0 );

	XMVECTOR down_rotate = XMVectorSet( 0.0, 0.0, 0.0, 0.0 );
	XMVECTOR left_rotate = XMVectorSet( 0.0, 0.0, 0.0, 0.0 );

	//ruch do przodu wzd³u¿ kierunku patrzenia, albo do ty³u
	if ( buttonState[PROTOTYPE_BUTTONS::FORWARD] && !buttonState[PROTOTYPE_BUTTONS::BACKWARD] )
	{
		XMVECTOR versor = XMVectorSet( 0.0, 0.0, -1.0, 0.0 );
		XMVECTOR orientation = object->GetOrientation( );
		XMMATRIX orient = XMMatrixRotationQuaternion( orientation );
		forward = XMVector4Transform( versor, orient );
		forward *= move_speed;
	}
	else if ( buttonState[PROTOTYPE_BUTTONS::BACKWARD] && !buttonState[PROTOTYPE_BUTTONS::FORWARD] )
	{
		XMVECTOR versor = XMVectorSet( 0.0, 0.0, 1.0, 0.0 );
		XMVECTOR orientation = object->GetOrientation( );
		XMMATRIX orient = XMMatrixRotationQuaternion( orientation );
		forward = XMVector4Transform( versor, orient );
		forward *= move_speed;
	}

	//ruch w lewo albo w prawo
	if ( buttonState[PROTOTYPE_BUTTONS::LEFT] && !buttonState[PROTOTYPE_BUTTONS::RIGHT] )
	{
		XMVECTOR versor = XMVectorSet( -1.0, 0.0, 0.0, 0.0 );
		XMVECTOR orientation = object->GetOrientation( );
		XMMATRIX orient = XMMatrixRotationQuaternion( orientation );
		left = XMVector4Transform( versor, orient );
		left *= move_speed;
	}
	else if ( buttonState[PROTOTYPE_BUTTONS::RIGHT] && !buttonState[PROTOTYPE_BUTTONS::LEFT] )
	{
		XMVECTOR versor = XMVectorSet( 1.0, 0.0, 0.0, 0.0 );
		XMVECTOR orientation = object->GetOrientation( );
		XMMATRIX orient = XMMatrixRotationQuaternion( orientation );
		left = XMVector4Transform( versor, orient );
		left *= move_speed;
	}

	forward = forward + left;		//dodajemy ruch posuwisty
	object->SetSpeed( forward );	//nadajemy obiektowi prêdkoœæ, poprzedni¹ siê nie przejmujemy

	//ruch obrotowy
	//mamy lewoskrêtny uk³ad wspó³rzednych, wiêc wektory s¹ odwrotnie zwrócone wzglêdem normalnych wektorów
	if ( buttonState[PROTOTYPE_BUTTONS::DOWN] && !buttonState[PROTOTYPE_BUTTONS::UP] )
	{
		XMVECTOR versor = XMVectorSet( -1.0, 0.0, 0.0, 0.0 );
		XMVECTOR orientation = object->GetOrientation( );
		XMMATRIX orient = XMMatrixRotationQuaternion( orientation );
		down_rotate = XMVector4Transform( versor, orient );
		down_rotate *= button_rot_speed;
	}
	else if ( buttonState[PROTOTYPE_BUTTONS::UP] && !buttonState[PROTOTYPE_BUTTONS::DOWN] )
	{
		XMVECTOR versor = XMVectorSet( 1.0, 0.0, 0.0, 0.0 );
		XMVECTOR orientation = object->GetOrientation( );
		XMMATRIX orient = XMMatrixRotationQuaternion( orientation );
		down_rotate = XMVector4Transform( versor, orient );
		down_rotate *= button_rot_speed;
	}

	if ( buttonState[PROTOTYPE_BUTTONS::TURN_LEFT] && !buttonState[PROTOTYPE_BUTTONS::TURN_RIGHT] )
	{
		left_rotate = XMVectorSet( 0.0, 1.0, 0.0, 0.0 );
		//zawsze obracamy wzglêdem osi pionowej, dlatego nie mno¿ymy przez kwaternion orientacji
		left_rotate *= button_rot_speed;
	}
	else if ( buttonState[PROTOTYPE_BUTTONS::TURN_RIGHT] && !buttonState[PROTOTYPE_BUTTONS::TURN_LEFT] )
	{
		left_rotate = XMVectorSet( 0.0, -1.0, 0.0, 0.0 );
		//zawsze obracamy wzglêdem osi pionowej, dlatego nie mno¿ymy przez kwaternion orientacji
		left_rotate *= button_rot_speed;
	}

	//pobieramy tablicê osi
	auto& axis_state = abstraction_layer->GetAxisTable();

	if( buttonState[ PROTOTYPE_BUTTONS::RIGHT_CLICK ] )
	{
		float y_axis = axis_state[ PROTOTYPE_AXES::Y_AXIS ];
		if( y_axis != 0.0f )
		{
			XMVECTOR versor = XMVectorSet( -1.0, 0.0, 0.0, 0.0 );
			XMVECTOR orientation = object->GetOrientation();
			XMMATRIX orient = XMMatrixRotationQuaternion( orientation );
			XMVECTOR axis_down_rotate = XMVector4Transform( versor, orient );
			axis_down_rotate *= y_axis * axis_rot_speed;

			down_rotate += axis_down_rotate;
		}

		float x_axis = axis_state[ PROTOTYPE_AXES::X_AXIS ];
		if( x_axis != 0.0f )
		{
			XMVECTOR axis_left_rotate = XMVectorSet( 0.0, -1.0, 0.0, 0.0 );
			axis_left_rotate *= x_axis * axis_rot_speed;

			left_rotate += axis_left_rotate;
		}
	}

	down_rotate = down_rotate + left_rotate;				//obroty siê dodaj¹ jak wektory
	XMVECTOR length = XMVector3Length( down_rotate );		//d³ugoœæ wektora jest jednoczeœnie szybkoœci¹ obrotu
	float f_length = XMVectorGetX( length );
	down_rotate = XMVectorSetW( down_rotate, f_length );	//zapisujemy szybkoœæ obrotu w sk³adowej w

	object->SetRotationSpeed( down_rotate );

	
}
