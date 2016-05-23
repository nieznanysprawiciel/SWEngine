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
