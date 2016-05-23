#include "EngineCore/stdafx.h"
#include "KeyboardState.h"


/**@brief */
KeyboardState::KeyboardState()
{
	for( auto& val : m_keyboardState )
		val = 0;
}

/**@brief */
KeyboardState::~KeyboardState()
{ }
