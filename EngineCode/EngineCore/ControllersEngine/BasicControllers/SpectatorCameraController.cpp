#include "EngineCore/stdafx.h"
#include "SpectatorCameraController.h"

#include "EngineCore/UIEngine/InputAbstractionLayer.h"
#include "EngineCore/Actors/BasicActors/DynamicActor.h"

#include "swCommonLib/Common/MemoryLeaks.h"


RTTR_REGISTRATION
{
	rttr::registration::class_< sw::SpectatorCameraController >( "SpectatorCameraController" );
}


using namespace DirectX;

namespace sw
{



/**@brief */
SpectatorCameraController::SpectatorCameraController( InputAbstractionLayerBase* layer )
	: BaseInputController( layer )
{
	m_moveSpeed = 1000.0;
	m_buttonRotSpeed = 1.0;
	m_axisRotSpeed = 120.0;
	m_zoomSpeed = 100.0;
}

/**@brief */
SpectatorCameraController::~SpectatorCameraController()
{}


using namespace STANDARD_LAYERS;

/*Funkcja g³ówna odpowiedzialna za sterowanie ruchem obiektu.*/
void SpectatorCameraController::ControlObjectPre( DynamicActor* actor, IControllersState* globalState )
{
	if( !m_abstractionLayer->IsActive() )
		return;
	//pobieramy tablicê przycisków
	auto& buttonState = m_abstractionLayer->GetButtonsTable();
	auto& axesState = m_abstractionLayer->GetAxisTable();

	XMVECTOR forward = XMVectorSet( 0.0, 0.0, 0.0, 0.0 );
	XMVECTOR left = XMVectorSet( 0.0, 0.0, 0.0, 0.0 );
	XMVECTOR up = XMVectorSet( 0.0, 0.0, 0.0, 0.0 );

	XMVECTOR downRotate = XMVectorSet( 0.0, 0.0, 0.0, 0.0 );
	XMVECTOR leftRotate = XMVectorSet( 0.0, 0.0, 0.0, 0.0 );

	//ruch do przodu wzd³u¿ kierunku patrzenia, albo do ty³u
	if( buttonState[ PROTOTYPE_BUTTONS::FORWARD ] && !buttonState[ PROTOTYPE_BUTTONS::BACKWARD ] )
	{
		XMVECTOR versor = XMVectorSet( 0.0, 0.0, -1.0, 0.0 );
		XMVECTOR orientation = actor->GetOrientation();
		XMMATRIX orient = XMMatrixRotationQuaternion( orientation );
		forward = XMVector4Transform( versor, orient );
		forward *= m_moveSpeed;
	}
	else if( buttonState[ PROTOTYPE_BUTTONS::BACKWARD ] && !buttonState[ PROTOTYPE_BUTTONS::FORWARD ] )
	{
		XMVECTOR versor = XMVectorSet( 0.0, 0.0, 1.0, 0.0 );
		XMVECTOR orientation = actor->GetOrientation();
		XMMATRIX orient = XMMatrixRotationQuaternion( orientation );
		forward = XMVector4Transform( versor, orient );
		forward *= m_moveSpeed;
	}

	if( axesState[ PROTOTYPE_AXES::ZOOM ] != 0 )
	{
		XMVECTOR versor = XMVectorSet( 0.0, 0.0, -1.0, 0.0 );
		XMVECTOR orientation = actor->GetOrientation();
		XMMATRIX orient = XMMatrixRotationQuaternion( orientation );
		forward += XMVector4Transform( versor, orient ) * m_zoomSpeed * axesState[ PROTOTYPE_AXES::ZOOM ];
	}

	//ruch w lewo albo w prawo
	if( buttonState[ PROTOTYPE_BUTTONS::LEFT ] && !buttonState[ PROTOTYPE_BUTTONS::RIGHT ] )
	{
		XMVECTOR versor = XMVectorSet( -1.0, 0.0, 0.0, 0.0 );
		XMVECTOR orientation = actor->GetOrientation();
		XMMATRIX orient = XMMatrixRotationQuaternion( orientation );
		left = XMVector4Transform( versor, orient );
		left *= m_moveSpeed;
	}
	else if( buttonState[ PROTOTYPE_BUTTONS::RIGHT ] && !buttonState[ PROTOTYPE_BUTTONS::LEFT ] )
	{
		XMVECTOR versor = XMVectorSet( 1.0, 0.0, 0.0, 0.0 );
		XMVECTOR orientation = actor->GetOrientation();
		XMMATRIX orient = XMMatrixRotationQuaternion( orientation );
		left = XMVector4Transform( versor, orient );
		left *= m_moveSpeed;
	}

	// Ruch w górê oraz w dó³ po osi Y
	if( buttonState[ PROTOTYPE_BUTTONS::UP ] && !buttonState[ PROTOTYPE_BUTTONS::DOWN ] )
		up = XMVectorSet( 0.0, 1.0, 0.0, 0.0 ) * m_moveSpeed;
	else if( buttonState[ PROTOTYPE_BUTTONS::DOWN ] && !buttonState[ PROTOTYPE_BUTTONS::UP ] )
		up = XMVectorSet( 0.0, -1.0, 0.0, 0.0 ) * m_moveSpeed;


	forward = forward + left + up;		//dodajemy ruch posuwisty
	actor->SetSpeed( forward );			//nadajemy obiektowi prêdkoœæ, poprzedni¹ siê nie przejmujemy

	//ruch obrotowy
	//mamy lewoskrêtny uk³ad wspó³rzednych, wiêc wektory s¹ odwrotnie zwrócone wzglêdem normalnych wektorów
	if( buttonState[ PROTOTYPE_BUTTONS::TURN_DOWN ] && !buttonState[ PROTOTYPE_BUTTONS::TURN_UP ] )
	{
		XMVECTOR versor = XMVectorSet( -1.0, 0.0, 0.0, 0.0 );
		XMVECTOR orientation = actor->GetOrientation();
		XMMATRIX orient = XMMatrixRotationQuaternion( orientation );
		downRotate = XMVector4Transform( versor, orient );
		downRotate *= m_buttonRotSpeed;
	}
	else if( buttonState[ PROTOTYPE_BUTTONS::TURN_UP ] && !buttonState[ PROTOTYPE_BUTTONS::TURN_DOWN ] )
	{
		XMVECTOR versor = XMVectorSet( 1.0, 0.0, 0.0, 0.0 );
		XMVECTOR orientation = actor->GetOrientation();
		XMMATRIX orient = XMMatrixRotationQuaternion( orientation );
		downRotate = XMVector4Transform( versor, orient );
		downRotate *= m_buttonRotSpeed;
	}

	if( buttonState[ PROTOTYPE_BUTTONS::TURN_LEFT ] && !buttonState[ PROTOTYPE_BUTTONS::TURN_RIGHT ] )
	{
		leftRotate = XMVectorSet( 0.0, 1.0, 0.0, 0.0 );
		//zawsze obracamy wzglêdem osi pionowej, dlatego nie mno¿ymy przez kwaternion orientacji
		leftRotate *= m_buttonRotSpeed;
	}
	else if( buttonState[ PROTOTYPE_BUTTONS::TURN_RIGHT ] && !buttonState[ PROTOTYPE_BUTTONS::TURN_LEFT ] )
	{
		leftRotate = XMVectorSet( 0.0, -1.0, 0.0, 0.0 );
		//zawsze obracamy wzglêdem osi pionowej, dlatego nie mno¿ymy przez kwaternion orientacji
		leftRotate *= m_buttonRotSpeed;
	}

	//pobieramy tablicê osi
	auto& axis_state = m_abstractionLayer->GetAxisTable();

	if( buttonState[ PROTOTYPE_BUTTONS::RIGHT_CLICK ] )
	{
		float y_axis = axis_state[ PROTOTYPE_AXES::Y_AXIS ];
		if( y_axis != 0.0f )
		{
			XMVECTOR versor = XMVectorSet( -1.0, 0.0, 0.0, 0.0 );
			XMVECTOR orientation = actor->GetOrientation();
			XMMATRIX orient = XMMatrixRotationQuaternion( orientation );
			XMVECTOR axis_down_rotate = XMVector4Transform( versor, orient );
			axis_down_rotate *= y_axis * m_axisRotSpeed;

			downRotate += axis_down_rotate;
		}

		float x_axis = axis_state[ PROTOTYPE_AXES::X_AXIS ];
		if( x_axis != 0.0f )
		{
			XMVECTOR axis_left_rotate = XMVectorSet( 0.0, -1.0, 0.0, 0.0 );
			axis_left_rotate *= x_axis * m_axisRotSpeed;

			leftRotate += axis_left_rotate;
		}
	}

	downRotate = downRotate + leftRotate;				//obroty siê dodaj¹ jak wektory
	XMVECTOR length = XMVector3Length( downRotate );		//d³ugoœæ wektora jest jednoczeœnie szybkoœci¹ obrotu
	float f_length = XMVectorGetX( length );
	downRotate = XMVectorSetW( downRotate, f_length );	//zapisujemy szybkoœæ obrotu w sk³adowej w

	actor->SetRotationSpeed( downRotate );


}

/**@brief Funkcja nic nie robi.

W trybie debug funkcja zatrzymuje siê na assercie.*/
void SpectatorCameraController::ControlObjectPost( DynamicActor* actor, IControllersState* globalState )
{
	assert( !"This is only pre controlled class." );
}

// ================================ //
//
void	SpectatorCameraController::Initialize		( DynamicActor* actor )
{}

}	// sw