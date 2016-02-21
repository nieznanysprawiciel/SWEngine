#pragma once

#include "ActorInfo.h"

#include <string>

/**@brief Struktura przechowuj�ca informacje o presetach aktor�w.

Na podstawie presetu mo�na stworzy� aktora.*/
struct ActorPreset
{
	std::string		presetName;
	std::string		meshPath;
	// Controller
	// Physic component
	// Collision component
	ActorInfo		addToModules;
};


