#pragma once

#include "InputDeviceInfo.h"

class MouseState
{
private:

	InputDeviceInfo		m_info;

	float				m_axes[ 4 ];
	char				m_buttons[ 8 ];

public:
	MouseState();
	~MouseState();

	const float*		GetAxesState()		{ return m_axes; }
	const char*			GetButtonsState()	{ return m_buttons; }

	const InputDeviceInfo&		GetInfo()	{ return m_info; }
};

