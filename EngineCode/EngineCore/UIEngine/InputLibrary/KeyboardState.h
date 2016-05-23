#pragma once
/**
@file KeyboardState.h
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.
*/


#include "InputDeviceInfo.h"


class KeyboardState
{
private:

	InputDeviceInfo		m_info;
	char				m_keyboardState[ 256 ];

public:
	explicit KeyboardState();
	~KeyboardState();

	const char*			GetKeyboardState()		{ return m_keyboardState; }

	const InputDeviceInfo&		GetInfo()	{ return m_info; }
};

