#include "EngineCore/stdafx.h"
#include "EngineCore/UIEngine/StandardAbstractionLayers.h"


namespace STANDARD_LAYERS
{

	input_mapping PROTOTYPE_BUTTONS_mapping[PROTOTYPE_BUTTONS_MAPPING_COUNT] =
	{

		{ DEVICE_IDs::KEYBOARD, PROTOTYPE_BUTTONS::FORWARD, DIK_W },
		{ DEVICE_IDs::KEYBOARD, PROTOTYPE_BUTTONS::BACKWARD, DIK_S },
		{ DEVICE_IDs::KEYBOARD, PROTOTYPE_BUTTONS::LEFT, DIK_A },
		{ DEVICE_IDs::KEYBOARD, PROTOTYPE_BUTTONS::RIGHT, DIK_D },
		{ DEVICE_IDs::KEYBOARD, PROTOTYPE_BUTTONS::TURN_RIGHT, DIK_X },
		{ DEVICE_IDs::KEYBOARD, PROTOTYPE_BUTTONS::TURN_LEFT, DIK_Z },
		{ DEVICE_IDs::KEYBOARD, PROTOTYPE_BUTTONS::UP, DIK_Q },
		{ DEVICE_IDs::KEYBOARD, PROTOTYPE_BUTTONS::DOWN, DIK_E },
		{ DEVICE_IDs::KEYBOARD, PROTOTYPE_BUTTONS::SHOT1, DIK_LCONTROL },
		{ DEVICE_IDs::KEYBOARD, PROTOTYPE_BUTTONS::SHOT2, DIK_RCONTROL },
		{ DEVICE_IDs::KEYBOARD, PROTOTYPE_BUTTONS::ESCAPE, DIK_ESCAPE },
		{ DEVICE_IDs::KEYBOARD, PROTOTYPE_BUTTONS::GENERATE_LIGHTMAPS1, DIK_1 },
		{ DEVICE_IDs::KEYBOARD, PROTOTYPE_BUTTONS::GENERATE_LIGHTMAPS2, DIK_2 },
		{ DEVICE_IDs::KEYBOARD, PROTOTYPE_BUTTONS::GENERATE_LIGHTMAPS3, DIK_3 },
		{ DEVICE_IDs::KEYBOARD, PROTOTYPE_BUTTONS::GENERATE_LIGHTMAPS4, DIK_4 },
		{ DEVICE_IDs::KEYBOARD, PROTOTYPE_BUTTONS::GENERATE_LIGHTMAPS5, DIK_5 },
		{ DEVICE_IDs::KEYBOARD, PROTOTYPE_BUTTONS::LOAD_LIGHTMAP_SCENE, DIK_L },
	};

	input_mapping PROTOTYPE_AXES_mapping[PROTOTYPE_AXES_MAPPING_COUNT] =
	{
		{ DEVICE_IDs::MOUSE, PROTOTYPE_AXES::X_AXIS, DIMOUSE_XAXIS },
		{ DEVICE_IDs::MOUSE, PROTOTYPE_AXES::Y_AXIS, DIMOUSE_YAXIS }, 
		{ DEVICE_IDs::MOUSE, PROTOTYPE_AXES::Z_AXIS, DIMOUSE_WHEEL }
	};

}
