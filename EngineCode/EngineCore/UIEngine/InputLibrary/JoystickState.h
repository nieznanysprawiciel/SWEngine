#pragma once
/**
@file JoystickState.h
@author nieznanysprawiciel
@copyright Plik jest cz�ci� silnika graficznego SWEngine.
*/


#include "InputDeviceInfo.h"


/**@brief Stan joysticka lub innego urz�dzenia wej�cia, kt�re nie jest
klawiatur� lub mysz�.*/
class JoystickState
{
private:

	InputDeviceInfo		m_info;


public:
	JoystickState();
	~JoystickState();

	const InputDeviceInfo&		GetInfo()	{ return m_info; }
};

