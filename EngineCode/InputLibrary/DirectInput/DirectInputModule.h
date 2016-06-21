#pragma once

#include "InputLibrary/IInput.h"
#include "IncludeDirectInput.h"

#include <vector>


/**@brief Klasa obs�uguj�ca wej�cie u�ytkownika.

U�ywa interfejsu Direct Inputa.

@todo Zrobi� obs�ug� wielu urz�dze� tego samego typu i jousticka.
@todo Pozby� si� includowania direct input z EngineCore.*/
class DirectInputModule : public IInput
{
private:

	HWND						m_windowHandle;

	LPDIRECTINPUT8				m_directInput;
	LPDIRECTINPUTDEVICE8		m_keyboardInput;		///<@todo Pewnie to powienien by� wektor.
	LPDIRECTINPUTDEVICE8		m_mouseInput;			///<@todo Pewnie to powienien by� wektor.

	std::vector< KeyboardState* >	m_keyboards;
	std::vector< MouseState* >		m_mouses;
	std::vector< JoystickState* >	m_joysticks;

public:
	explicit										DirectInputModule	();
	virtual											~DirectInputModule	();

	virtual bool									Init				( const InputInitInfo& initInfo ) override;

	virtual const std::vector< KeyboardState* >&	GetKeyboardStates	() override;
	virtual const std::vector< MouseState* >&		GetMouseStates		() override;
	virtual const std::vector< JoystickState* >&	GetJoystickStates	() override;

	virtual std::vector< const InputDeviceInfo* >	GetDevicesInfo		() override;

	virtual void									Update				( float timeInterval ) override;
	virtual bool									UpdateDevices		() override;

private:

	void			CleanDirectInput	();
	void			ClearInputStates	();

	void			UpdateKeyboard		( int idx );
	void			UpdateMouse			( int idx );
	void			UpdateJoystick		( int idx );
};

