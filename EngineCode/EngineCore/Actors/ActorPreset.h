#pragma once

#include "ActorInfo.h"

#include <string>

/**@brief Struktura przechowuj¹ca informacje o presetach aktorów.

Na podstawie presetu mo¿na stworzyæ aktora.*/
struct ActorPreset
{
	std::string		presetName;
	std::string		meshPath;
	// Controller
	// Physic component
	// Collision component
	ActorInfo		addToModules;
};


