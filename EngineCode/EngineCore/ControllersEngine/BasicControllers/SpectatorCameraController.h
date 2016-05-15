#pragma once

/**@file camera_controller_PROTOTYPE
@brief Plik zawiera deklaracj� kontrolera przeznaczonego do kamery zgodnego z wartstw� abstrakcji
PROTOTYPE.*/

#include "EngineCore/ControllersEngine/BaseClasses/BaseInputController.h"


/**@brief Kontroler kamery zgodny z wartw� abstrakcji PROTOTYPE.

Kontroler implementuje podstawowe operacje
jak przesuwanie w prz�d, ty�, obr�t g�ra, d� i br�t wzgl�dem osi pionowej.
Pr�dko�� poszczeg�lnych ruch�w jest sta�a i zale�na od zmiennych m_moveSpeed i m_buttonRotSpeed.

Sterowanie:
- PROTOTYPE::FORWARD Ruch do przodu wzd�u� kierunku patrzenia
- PROTOTYPE::BACKWARD Ruch do ty�u wzd�u� kierunku patrzenia
- PROTOTYPE::LEFT Ruch w lewo Prostopadle do kierunku patrzenia (nie obraca kamery)
- PROTOTYPE::RIGHT Ruch w prawo Prostopadle do kierunku patrzenia (nie obraca kamery)
- PROTOTYPE::DOWN Obr�t w d�
- PROTOTYPE::UP Obr�t w g�r�
- PROTOTYPE::TURN_LEFT Obr�t w lewo wok� osi pionowej
- PROTOTYPE::TURN_RIGHT Obr�t w prawo wok� osi pionowej
*/
class SpectatorCameraController	:	public BaseInputController
{
	RTTR_ENABLE( BaseInputController )
private:
	float		m_moveSpeed;			///<Pr�dko�� poruszania posuwistego
	float		m_buttonRotSpeed;		///<Pr�dko�� ruchu obrotowego (dla przycisku)
	float		m_axisRotSpeed;			///<Pr�dko�� ruchu obrotowego (dla myszy/jousticka)
public:
	explicit SpectatorCameraController			( InputAbstractionLayerBase* layer );
	virtual ~SpectatorCameraController			();

	virtual void		ControlObjectPre		( DynamicActor* );
	virtual void		ControlObjectPost		( DynamicActor* );
};

