#pragma once

/**@file camera_controller_PROTOTYPE
@brief Plik zawiera deklaracj� kontrolera przeznaczonego do kamery zgodnego z wartstw� abstrakcji
PROTOTYPE.*/

#include "EngineCore/ControllersEngine/BaseClasses/BaseInputController.h"



namespace sw
{

/**@brief Kontroler kamery zgodny z wartw� abstrakcji PROTOTYPE.

Kontroler implementuje podstawowe operacje
jak przesuwanie w prz�d, ty�, obr�t g�ra, d� i br�t wzgl�dem osi pionowej.
Pr�dko�� poszczeg�lnych ruch�w jest sta�a i zale�na od zmiennych m_moveSpeed i m_buttonRotSpeed.

Sterowanie:
- PROTOTYPE_BUTTONS::FORWARD Ruch do przodu wzd�u� kierunku patrzenia
- PROTOTYPE_BUTTONS::BACKWARD Ruch do ty�u wzd�u� kierunku patrzenia
- PROTOTYPE_BUTTONS::LEFT Ruch w lewo Prostopadle do kierunku patrzenia (nie obraca kamery)
- PROTOTYPE_BUTTONS::RIGHT Ruch w prawo Prostopadle do kierunku patrzenia (nie obraca kamery)
- PROTOTYPE_BUTTONS::TURN_DOWN Obr�t w d�
- PROTOTYPE_BUTTONS::TURN_UP Obr�t w g�r�
- PROTOTYPE_BUTTONS::TURN_LEFT Obr�t w lewo wok� osi pionowej
- PROTOTYPE_BUTTONS::TURN_RIGHT Obr�t w prawo wok� osi pionowej
- PROTOTYPE_BUTTONS::UP Przesuni�cie w g�r� po osi Y
- PROTOTYPE_BUTTONS::DOWN Przesuni�cie w d� po osi Y
- PROTOTYPE_AXES::ZOOM Przybli�anie i oddalanie (ruch wzd�u� kierunku patrzenia)
*/
class SpectatorCameraController : public BaseInputController
{
	RTTR_ENABLE( BaseInputController )
private:
	float		m_moveSpeed;			///<Pr�dko�� poruszania posuwistego
	float		m_buttonRotSpeed;		///<Pr�dko�� ruchu obrotowego (dla przycisku)
	float		m_axisRotSpeed;			///<Pr�dko�� ruchu obrotowego (dla myszy/jousticka)
	float		m_zoomSpeed;			///<Pr�dko�� zbli�ania i oddalania (k�kiem myszy)
public:
	explicit SpectatorCameraController			( InputAbstractionLayerBase* layer );
	virtual ~SpectatorCameraController			();

	virtual void		ControlObjectPre		( DynamicActor* actor, IControllersState* globalState );
	virtual void		ControlObjectPost		( DynamicActor* actor, IControllersState* globalState );

	virtual void		Initialize				( DynamicActor* actor ) override;
};


}	// sw

