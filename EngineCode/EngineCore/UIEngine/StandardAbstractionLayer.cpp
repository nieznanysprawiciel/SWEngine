#include "EngineCore/stdafx.h"
#include "EngineCore/UIEngine/StandardAbstractionLayers.h"

#include "swInputLibrary/InputCore/KeyboardState.h"
#include "swInputLibrary/InputCore/MouseState.h"

#include <vector>



namespace STANDARD_LAYERS
{

	std::vector< InputMapping > PROTOTYPE_BUTTONS_mapping =
	{

		{ DEVICE_IDs::KEYBOARD, PROTOTYPE_BUTTONS::FORWARD, sw::input::Keyboard::PhysicalKeys::KEY_W },
		{ DEVICE_IDs::KEYBOARD, PROTOTYPE_BUTTONS::BACKWARD, sw::input::Keyboard::PhysicalKeys::KEY_S },
		{ DEVICE_IDs::KEYBOARD, PROTOTYPE_BUTTONS::LEFT, sw::input::Keyboard::PhysicalKeys::KEY_A },
		{ DEVICE_IDs::KEYBOARD, PROTOTYPE_BUTTONS::RIGHT, sw::input::Keyboard::PhysicalKeys::KEY_D },
		{ DEVICE_IDs::KEYBOARD, PROTOTYPE_BUTTONS::TURN_RIGHT, sw::input::Keyboard::PhysicalKeys::KEY_E },
		{ DEVICE_IDs::KEYBOARD, PROTOTYPE_BUTTONS::TURN_LEFT, sw::input::Keyboard::PhysicalKeys::KEY_Q },
		{ DEVICE_IDs::KEYBOARD, PROTOTYPE_BUTTONS::UP, sw::input::Keyboard::PhysicalKeys::KEY_R },
		{ DEVICE_IDs::KEYBOARD, PROTOTYPE_BUTTONS::DOWN, sw::input::Keyboard::PhysicalKeys::KEY_F },
		{ DEVICE_IDs::KEYBOARD, PROTOTYPE_BUTTONS::TURN_UP, sw::input::Keyboard::PhysicalKeys::KEY_T },
		{ DEVICE_IDs::KEYBOARD, PROTOTYPE_BUTTONS::TURN_DOWN, sw::input::Keyboard::PhysicalKeys::KEY_G },


		{ DEVICE_IDs::KEYBOARD, PROTOTYPE_BUTTONS::SHOT1, sw::input::Keyboard::PhysicalKeys::KEY_LCONTROL },
		{ DEVICE_IDs::KEYBOARD, PROTOTYPE_BUTTONS::SHOT2, sw::input::Keyboard::PhysicalKeys::KEY_RCONTROL },

		{ DEVICE_IDs::KEYBOARD, PROTOTYPE_BUTTONS::ESCAPE, sw::input::Keyboard::PhysicalKeys::KEY_ESCAPE },

		{ DEVICE_IDs::KEYBOARD, PROTOTYPE_BUTTONS::GENERATE_LIGHTMAPS1, sw::input::Keyboard::PhysicalKeys::KEY_1 },
		{ DEVICE_IDs::KEYBOARD, PROTOTYPE_BUTTONS::GENERATE_LIGHTMAPS2, sw::input::Keyboard::PhysicalKeys::KEY_2 },
		{ DEVICE_IDs::KEYBOARD, PROTOTYPE_BUTTONS::GENERATE_LIGHTMAPS3, sw::input::Keyboard::PhysicalKeys::KEY_3 },
		{ DEVICE_IDs::KEYBOARD, PROTOTYPE_BUTTONS::GENERATE_LIGHTMAPS4, sw::input::Keyboard::PhysicalKeys::KEY_4 },
		{ DEVICE_IDs::KEYBOARD, PROTOTYPE_BUTTONS::GENERATE_LIGHTMAPS5, sw::input::Keyboard::PhysicalKeys::KEY_5 },
		{ DEVICE_IDs::KEYBOARD, PROTOTYPE_BUTTONS::LOAD_LIGHTMAP_SCENE, sw::input::Keyboard::PhysicalKeys::KEY_L },

		{ DEVICE_IDs::KEYBOARD, PROTOTYPE_BUTTONS::ENABLE_MOVE_X, sw::input::Keyboard::PhysicalKeys::KEY_X },
		{ DEVICE_IDs::KEYBOARD, PROTOTYPE_BUTTONS::ENABLE_MOVE_Y, sw::input::Keyboard::PhysicalKeys::KEY_Y },
		{ DEVICE_IDs::KEYBOARD, PROTOTYPE_BUTTONS::ENABLE_MOVE_Z, sw::input::Keyboard::PhysicalKeys::KEY_Z },

		{ DEVICE_IDs::MOUSE, PROTOTYPE_BUTTONS::RIGHT_CLICK, sw::input::Mouse::PhysicalButtons::RIGHT_BUTTON },
		{ DEVICE_IDs::MOUSE, PROTOTYPE_BUTTONS::LEFT_CLICK, sw::input::Mouse::PhysicalButtons::LEFT_BUTTON },
	};

	std::vector< InputMapping > PROTOTYPE_AXES_mapping =
	{
		{ DEVICE_IDs::MOUSE, PROTOTYPE_AXES::X_AXIS, sw::input::Mouse::PhysicalAxes::X_AXIS },
		{ DEVICE_IDs::MOUSE, PROTOTYPE_AXES::Y_AXIS, sw::input::Mouse::PhysicalAxes::Y_AXIS },
		{ DEVICE_IDs::MOUSE, PROTOTYPE_AXES::ZOOM, sw::input::Mouse::PhysicalAxes::WHEEL }
	};

}

