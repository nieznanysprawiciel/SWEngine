#include "EngineCore/stdafx.h"
#include "MouseState.h"


/**@brief */
MouseState::MouseState()
{
	for( auto& val : m_axes )
		val = 0.0f;
	for( auto& val : m_buttons )
		val = 0;
}

/**@brief */
MouseState::~MouseState()
{ }

/**@brief Ustawia pozycjê myszy.

Powinno byæ u¿ywane jedynie przez modu³ wejœcia (@ref IInput).*/
void			MouseState::SetPosition			( short X, short Y )
{
	m_position[ 0 ] = X;
	m_position[ 1 ] = Y;
}
