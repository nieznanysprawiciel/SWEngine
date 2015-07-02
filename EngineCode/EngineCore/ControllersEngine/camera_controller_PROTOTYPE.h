#pragma once

/**@file camera_controller_PROTOTYPE
@brief Plik zawiera deklaracjê kontrolera przeznaczonego do kamery zgodnego z wartstw¹ anstrakcji
PROTOTYPE.*/

#include "Interfaces\basic_interfaces.h"


/**@brief Kontroler kamery zgodny z wartw¹ abstrakcji PROTOTYPE.

Kontroler implementuje podstawowe operacje
jak przesuwanie w przód, ty³, obrót góra, dó³ i brót wzglêdem osi pionowej.
Prêdkoœæ poszczególnych ruchów jest sta³a i zale¿na od zmiennych move_speed i button_rot_speed.

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
class camera_controller_PROTOTYPE	:	public BaseInputController
{
private:
	float move_speed;			///<Prêdkoœæ poruszania posuwistego
	float button_rot_speed;		///<Prêdkoœæ ruchu obrotowego (dla przycisku)
	float axis_rot_speed;		///<Prêdkoœæ ruchu obrotowego (dla myszy/jousticka)
public:
	camera_controller_PROTOTYPE( InputAbstractionLayer_base* layer );
	virtual ~camera_controller_PROTOTYPE();

	virtual void control_object( DynamicObject* );
};

