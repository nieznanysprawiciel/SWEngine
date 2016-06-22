#pragma once

#include "InputDeviceInfo.h"

class MouseState
{
private:

	InputDeviceInfo		m_info;

	float				m_axes[ 4 ];
	short				m_position[ 2 ];	///< Wspó³rzêdne X i Y.
	char				m_buttons[ 8 ];

public:
	MouseState();
	~MouseState();

	float*						GetAxesState()		{ return m_axes; }
	char*						GetButtonsState()	{ return m_buttons; }

	short						GetPositionX()		{ return m_position[ 0 ]; }
	short						GetPositionY()		{ return m_position[ 1 ]; }

	void						SetPosition			( short X, short Y );

	const InputDeviceInfo&		GetInfo()			{ return m_info; }

public:

	/**@brief Przyciski fizyczne.
	
	Od BUTTON0 do BUTTON2 s¹ to przyciski prawy, lewy i œrodkowy, które
	maj¹ te¿ aliasy LEFT_BUTTON, RIGHT_BUTTON, MIDDLE_BUTTON.
	Kolejne numery oznaczaj¹ pozosta³e przyciski, jakie mo¿e posiadaæ mysz.*/
	enum PHYSICAL_BUTTONS
	{
		BUTTON0 = 0,
		BUTTON1 = 1,
		BUTTON2 = 2,
		BUTTON3 = 3,
		BUTTON4 = 4,
		BUTTON5 = 5,
		BUTTON6 = 6,
		BUTTON7 = 7,
		LEFT_BUTTON = BUTTON0,
		RIGHT_BUTTON = BUTTON1,
		MIDDLE_BUTTON = BUTTON2,

		NONE = 8,
	};

	enum PHYSICAL_AXES
	{
		X_AXIS = 0,
		Y_AXIS = 1,
		Z_AXIS = 2,
		W_AXIS = 3,
		WHEEL = Z_AXIS
	};

};
