#pragma once
/**@file IInput.h
@author nieznanysprawiciel
@copyright Plik jest cz�ci� silnika graficznego SWEngine.

@brief Interfejs dla klas pobieraj�cych dane o wej�ciu u�ytkownika.*/

#include "KeyboardState.h"
#include "MouseState.h"
#include "JoystickState.h"

#include "InputDeviceInfo.h"

#include <vector>

typedef void*	AppInstanceHandle;
typedef void*	WindowHandle;


/**@brief Zmienne parametryzuj�ce inicjalizacj� modu�u wej�cia wyj�cia.
Na razie nie u�ywane.*/
struct InputInitInfo
{
	AppInstanceHandle		AppInstance;
	WindowHandle			WndHandle;
};



/**@brief Interfejs dla klas pobieraj�cych dane o wej�ciu u�ytkownika.

@todo Uzupe�ni� interfejs i wydzieli� modu�y obs�uguj�ce konkretne implementacje
urz�dze� wej�ciowych.*/
class IInput
{
private:
protected:
public:
	virtual				~IInput() = 0	{};

	/**@brief Inicjalizuje ca�y modu�. Po wywo�aniu tej funkcji modu� musi by� gotowy do u�ytku
	w innym przypadku powinien zwr�ci� false.*/
	virtual bool									Init				( const InputInitInfo& initInfo ) = 0;

	virtual const std::vector< KeyboardState* >&	GetKeyboardStates	() = 0;
	virtual const std::vector< MouseState* >&		GetMouseStates		() = 0;
	virtual const std::vector< JoystickState* >&	GetJoystickStates	() = 0;

	virtual std::vector< const InputDeviceInfo* >	GetDevicesInfo		() = 0;

	virtual void									Update				( float timeInterval ) = 0;

	/**@brief Funkcja na nowo sprawdza obecne w systemie urz�dzenia i aktualizuje wszystkie
	potrzebne dane.*/
	virtual bool									UpdateDevices		() = 0;
};


