#include "EngineCore/stdafx.h"
#include "SpectatorCameraController.h"

#include "EngineCore/UIEngine/InputAbstractionLayer.h"
#include "EngineCore/Actors/BasicActors/DynamicActor.h"

#include "Common/MemoryLeaks.h"


RTTR_REGISTRATION
{
	rttr::registration::class_< SpectatorCameraController >( "SpectatorCameraController" );
}


using namespace DirectX;

SpectatorCameraController::SpectatorCameraController( InputAbstractionLayerBase* layer )
	:	BaseInputController(layer)
{
	m_moveSpeed = 1000.0;
	m_buttonRotSpeed = 1.0;
	m_axisRotSpeed = 120.0;
}


SpectatorCameraController::~SpectatorCameraController()
{
}


using namespace STANDARD_LAYERS;

/*Funkcja g��wna odpowiedzialna za sterowanie ruchem obiektu.*/
void SpectatorCameraController::ControlObjectPre( DynamicActor* object )
{
	if ( !m_abstractionLayer->IsActive() )
		return;
	//pobieramy tablic� przycisk�w
	auto& buttonState = m_abstractionLayer->GetButtonsTable();

	XMVECTOR forward = XMVectorSet( 0.0, 0.0, 0.0, 0.0 );
	XMVECTOR left = XMVectorSet( 0.0, 0.0, 0.0, 0.0 );

	XMVECTOR down_rotate = XMVectorSet( 0.0, 0.0, 0.0, 0.0 );
	XMVECTOR left_rotate = XMVectorSet( 0.0, 0.0, 0.0, 0.0 );

	//ruch do przodu wzd�u� kierunku patrzenia, albo do ty�u
	if ( buttonState[PROTOTYPE_BUTTONS::FORWARD] && !buttonState[PROTOTYPE_BUTTONS::BACKWARD] )
	{
		XMVECTOR versor = XMVectorSet( 0.0, 0.0, -1.0, 0.0 );
		XMVECTOR orientation = object->GetOrientation( );
		XMMATRIX orient = XMMatrixRotationQuaternion( orientation );
		forward = XMVector4Transform( versor, orient );
		forward *= m_moveSpeed;
	}
	else if ( buttonState[PROTOTYPE_BUTTONS::BACKWARD] && !buttonState[PROTOTYPE_BUTTONS::FORWARD] )
	{
		XMVECTOR versor = XMVectorSet( 0.0, 0.0, 1.0, 0.0 );
		XMVECTOR orientation = object->GetOrientation( );
		XMMATRIX orient = XMMatrixRotationQuaternion( orientation );
		forward = XMVector4Transform( versor, orient );
		forward *= m_moveSpeed;
	}

	//ruch w lewo albo w prawo
	if ( buttonState[PROTOTYPE_BUTTONS::LEFT] && !buttonState[PROTOTYPE_BUTTONS::RIGHT] )
	{
		XMVECTOR versor = XMVectorSet( -1.0, 0.0, 0.0, 0.0 );
		XMVECTOR orientation = object->GetOrientation( );
		XMMATRIX orient = XMMatrixRotationQuaternion( orientation );
		left = XMVector4Transform( versor, orient );
		left *= m_moveSpeed;
	}
	else if ( buttonState[PROTOTYPE_BUTTONS::RIGHT] && !buttonState[PROTOTYPE_BUTTONS::LEFT] )
	{
		XMVECTOR versor = XMVectorSet( 1.0, 0.0, 0.0, 0.0 );
		XMVECTOR orientation = object->GetOrientation( );
		XMMATRIX orient = XMMatrixRotationQuaternion( orientation );
		left = XMVector4Transform( versor, orient );
		left *= m_moveSpeed;
	}

	forward = forward + left;		//dodajemy ruch posuwisty
	object->SetSpeed( forward );	//nadajemy obiektowi pr�dko��, poprzedni� si� nie przejmujemy

	//ruch obrotowy
	//mamy lewoskr�tny uk�ad wsp�rzednych, wi�c wektory s� odwrotnie zwr�cone wzgl�dem normalnych wektor�w
	if ( buttonState[PROTOTYPE_BUTTONS::DOWN] && !buttonState[PROTOTYPE_BUTTONS::UP] )
	{
		XMVECTOR versor = XMVectorSet( -1.0, 0.0, 0.0, 0.0 );
		XMVECTOR orientation = object->GetOrientation( );
		XMMATRIX orient = XMMatrixRotationQuaternion( orientation );
		down_rotate = XMVector4Transform( versor, orient );
		down_rotate *= m_buttonRotSpeed;
	}
	else if ( buttonState[PROTOTYPE_BUTTONS::UP] && !buttonState[PROTOTYPE_BUTTONS::DOWN] )
	{
		XMVECTOR versor = XMVectorSet( 1.0, 0.0, 0.0, 0.0 );
		XMVECTOR orientation = object->GetOrientation( );
		XMMATRIX orient = XMMatrixRotationQuaternion( orientation );
		down_rotate = XMVector4Transform( versor, orient );
		down_rotate *= m_buttonRotSpeed;
	}

	if ( buttonState[PROTOTYPE_BUTTONS::TURN_LEFT] && !buttonState[PROTOTYPE_BUTTONS::TURN_RIGHT] )
	{
		left_rotate = XMVectorSet( 0.0, 1.0, 0.0, 0.0 );
		//zawsze obracamy wzgl�dem osi pionowej, dlatego nie mno�ymy przez kwaternion orientacji
		left_rotate *= m_buttonRotSpeed;
	}
	else if ( buttonState[PROTOTYPE_BUTTONS::TURN_RIGHT] && !buttonState[PROTOTYPE_BUTTONS::TURN_LEFT] )
	{
		left_rotate = XMVectorSet( 0.0, -1.0, 0.0, 0.0 );
		//zawsze obracamy wzgl�dem osi pionowej, dlatego nie mno�ymy przez kwaternion orientacji
		left_rotate *= m_buttonRotSpeed;
	}

	//pobieramy tablic� osi
	auto& axis_state = m_abstractionLayer->GetAxisTable();

	if( buttonState[ PROTOTYPE_BUTTONS::RIGHT_CLICK ] )
	{
		float y_axis = axis_state[ PROTOTYPE_AXES::Y_AXIS ];
		if( y_axis != 0.0f )
		{
			XMVECTOR versor = XMVectorSet( -1.0, 0.0, 0.0, 0.0 );
			XMVECTOR orientation = object->GetOrientation();
			XMMATRIX orient = XMMatrixRotationQuaternion( orientation );
			XMVECTOR axis_down_rotate = XMVector4Transform( versor, orient );
			axis_down_rotate *= y_axis * m_axisRotSpeed;

			down_rotate += axis_down_rotate;
		}

		float x_axis = axis_state[ PROTOTYPE_AXES::X_AXIS ];
		if( x_axis != 0.0f )
		{
			XMVECTOR axis_left_rotate = XMVectorSet( 0.0, -1.0, 0.0, 0.0 );
			axis_left_rotate *= x_axis * m_axisRotSpeed;

			left_rotate += axis_left_rotate;
		}
	}

	down_rotate = down_rotate + left_rotate;				//obroty si� dodaj� jak wektory
	XMVECTOR length = XMVector3Length( down_rotate );		//d�ugo�� wektora jest jednocze�nie szybko�ci� obrotu
	float f_length = XMVectorGetX( length );
	down_rotate = XMVectorSetW( down_rotate, f_length );	//zapisujemy szybko�� obrotu w sk�adowej w

	object->SetRotationSpeed( down_rotate );

	
}

/**@brief Funkcja nic nie robi.

W trybie debug funkcja zatrzymuje si� na assercie.*/
void SpectatorCameraController::ControlObjectPost( DynamicActor* object )
{
	assert( !"This is only pre controlled class." );
}
