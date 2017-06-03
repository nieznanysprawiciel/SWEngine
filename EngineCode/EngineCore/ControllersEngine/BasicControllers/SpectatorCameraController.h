#pragma once

/**@file camera_controller_PROTOTYPE
@brief Plik zawiera deklaracjê kontrolera przeznaczonego do kamery zgodnego z wartstw¹ abstrakcji
PROTOTYPE.*/

#include "EngineCore/ControllersEngine/BaseClasses/BaseInputController.h"



namespace sw
{

/**@brief Kontroler kamery zgodny z wartw¹ abstrakcji PROTOTYPE.

Kontroler implementuje podstawowe operacje
jak przesuwanie w przód, ty³, obrót góra, dó³ i brót wzglêdem osi pionowej.
Prêdkoœæ poszczególnych ruchów jest sta³a i zale¿na od zmiennych m_moveSpeed i m_buttonRotSpeed.

Sterowanie:
- PROTOTYPE_BUTTONS::FORWARD Ruch do przodu wzd³u¿ kierunku patrzenia
- PROTOTYPE_BUTTONS::BACKWARD Ruch do ty³u wzd³u¿ kierunku patrzenia
- PROTOTYPE_BUTTONS::LEFT Ruch w lewo Prostopadle do kierunku patrzenia (nie obraca kamery)
- PROTOTYPE_BUTTONS::RIGHT Ruch w prawo Prostopadle do kierunku patrzenia (nie obraca kamery)
- PROTOTYPE_BUTTONS::TURN_DOWN Obrót w dó³
- PROTOTYPE_BUTTONS::TURN_UP Obrót w górê
- PROTOTYPE_BUTTONS::TURN_LEFT Obrót w lewo wokó³ osi pionowej
- PROTOTYPE_BUTTONS::TURN_RIGHT Obrót w prawo wokó³ osi pionowej
- PROTOTYPE_BUTTONS::UP Przesuniêcie w górê po osi Y
- PROTOTYPE_BUTTONS::DOWN Przesuniêcie w dó³ po osi Y
- PROTOTYPE_AXES::ZOOM Przybli¿anie i oddalanie (ruch wzd³u¿ kierunku patrzenia)
*/
class SpectatorCameraController : public BaseInputController
{
	RTTR_ENABLE( BaseInputController )
private:
	float		m_moveSpeed;			///<Prêdkoœæ poruszania posuwistego
	float		m_buttonRotSpeed;		///<Prêdkoœæ ruchu obrotowego (dla przycisku)
	float		m_axisRotSpeed;			///<Prêdkoœæ ruchu obrotowego (dla myszy/jousticka)
	float		m_zoomSpeed;			///<Prêdkoœæ zbli¿ania i oddalania (kó³kiem myszy)
public:
	explicit SpectatorCameraController			( InputAbstractionLayerBase* layer );
	virtual ~SpectatorCameraController			();

	virtual void		ControlObjectPre		( DynamicActor* actor, IControllersState* globalState );
	virtual void		ControlObjectPost		( DynamicActor* actor, IControllersState* globalState );

	virtual void		Initialize				( DynamicActor* actor ) override;
};


}	// sw

