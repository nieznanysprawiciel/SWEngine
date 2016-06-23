#pragma once

#include "InputDeviceInfo.h"
#include "KeyState.h"


#define MOUSE_STATE_BUTTONS_NUMBER 8

class MouseState
{
private:

	InputDeviceInfo		m_info;

	float				m_axes[ 4 ];
	short				m_position[ 2 ];	///< Wsp�rz�dne X i Y.
	KeyState			m_buttons[ MOUSE_STATE_BUTTONS_NUMBER ];

public:
	MouseState();
	~MouseState();

	float*						GetAxesState()		{ return m_axes; }
	KeyState*					GetButtonsState()	{ return m_buttons; }

	short						GetPositionX()		{ return m_position[ 0 ]; }
	short						GetPositionY()		{ return m_position[ 1 ]; }

	void						SetPosition			( short X, short Y );

	const InputDeviceInfo&		GetInfo()			{ return m_info; }

	///@name Funkcje do ustawiania stanu (tylko dla dzieci IInput)
	///@{
	void						RemoveEvents	();
	///@}

public:

	/**@brief Przyciski fizyczne.
	
	Od BUTTON0 do BUTTON2 s� to przyciski prawy, lewy i �rodkowy, kt�re
	maj� te� aliasy LEFT_BUTTON, RIGHT_BUTTON, MIDDLE_BUTTON.
	Kolejne numery oznaczaj� pozosta�e przyciski, jakie mo�e posiada� mysz.*/
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

/**@brief */
inline MouseState::MouseState()
{
	for( auto& val : m_axes )
		val = 0.0f;
	for( auto& val : m_buttons )
		val = 0;
}

/**@brief */
inline MouseState::~MouseState()
{ }

/**@brief Ustawia pozycj� myszy.

Powinno by� u�ywane jedynie przez modu� wej�cia (@ref IInput).*/
inline void			MouseState::SetPosition			( short X, short Y )
{
	m_position[ 0 ] = X;
	m_position[ 1 ] = Y;
}

/**@brief Czy�ci tablic� z event�w o wci�ni�ciu klawiszy, ale podtrzymuje stan przycisk�w.*/
inline void			MouseState::RemoveEvents	()
{
	for( int i = 0; i < MOUSE_STATE_BUTTONS_NUMBER; ++i )
		m_buttons[ i ].HoldState();
}

