#pragma once

/**@file camera_controller_PROTOTYPE
@brief Plik zawiera deklaracj� kontrolera przeznaczonego do kamery zgodnego z wartstw� anstrakcji
PROTOTYPE.*/

#include "..\..\Interfaces\basic_interfaces.h"


/**@brief Kontroler kamery zgodny z wartw� abstrakcji PROTOTYPE.

Kontroler implementuje podstawowe operacje
jak przesuwanie w prz�d, ty�, obr�t g�ra, d� i br�t wzgl�dem osi pionowej.
Pr�dko�� poszczeg�lnych ruch�w jest sta�a i zale�na od zmiennych move_speed i rot_speed.

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
class camera_controller_PROTOTYPE	:	public Base_input_controller
{
private:
	float move_speed;		///<Pr�dko�� poruszania posuwistego
	float rot_speed;		///<Pr�dko�� ruchu obrotowego
public:
	camera_controller_PROTOTYPE( InputAbstractionLayer_base* layer );
	virtual ~camera_controller_PROTOTYPE();

	virtual void control_object( Dynamic_object* );
};

