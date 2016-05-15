#pragma once

/**@file camera_controller_PROTOTYPE
@brief Plik zawiera deklaracjê kontrolera przeznaczonego do kamery zgodnego z wartstw¹ abstrakcji
PROTOTYPE.*/

#include "EngineCore/ControllersEngine/BaseClasses/BaseInputController.h"


/**@brief Kontroler kamery zgodny z wartw¹ abstrakcji PROTOTYPE.

Kontroler implementuje podstawowe operacje
jak przesuwanie w przód, ty³, obrót góra, dó³ i brót wzglêdem osi pionowej.
Prêdkoœæ poszczególnych ruchów jest sta³a i zale¿na od zmiennych m_moveSpeed i m_buttonRotSpeed.

Sterowanie:
- PROTOTYPE::FORWARD Ruch do przodu wzd³u¿ kierunku patrzenia
- PROTOTYPE::BACKWARD Ruch do ty³u wzd³u¿ kierunku patrzenia
- PROTOTYPE::LEFT Ruch w lewo Prostopadle do kierunku patrzenia (nie obraca kamery)
- PROTOTYPE::RIGHT Ruch w prawo Prostopadle do kierunku patrzenia (nie obraca kamery)
- PROTOTYPE::DOWN Obrót w dó³
- PROTOTYPE::UP Obrót w górê
- PROTOTYPE::TURN_LEFT Obrót w lewo wokó³ osi pionowej
- PROTOTYPE::TURN_RIGHT Obrót w prawo wokó³ osi pionowej
*/
class SpectatorCameraController	:	public BaseInputController
{
	RTTR_ENABLE( BaseInputController )
private:
	float		m_moveSpeed;			///<Prêdkoœæ poruszania posuwistego
	float		m_buttonRotSpeed;		///<Prêdkoœæ ruchu obrotowego (dla przycisku)
	float		m_axisRotSpeed;			///<Prêdkoœæ ruchu obrotowego (dla myszy/jousticka)
public:
	explicit SpectatorCameraController			( InputAbstractionLayerBase* layer );
	virtual ~SpectatorCameraController			();

	virtual void		ControlObjectPre		( DynamicActor* );
	virtual void		ControlObjectPost		( DynamicActor* );
};

