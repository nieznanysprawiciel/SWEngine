#pragma once
/**
@file ActorPreset.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/



#include "ActorInfo.h"

#include <string>




namespace sw
{


/**@brief Struktura przechowuj¹ca informacje o presetach aktorów.

Na podstawie presetu mo¿na stworzyæ aktora.*/
struct ActorPreset
{
	std::string		presetName;
	std::string		meshPath;
	// Controller
	// Physic component
	// Collision component
	// SubObjects
	ActorInfo		addToModules;
};

}	// sw

