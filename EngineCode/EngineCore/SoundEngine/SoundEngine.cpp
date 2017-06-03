/**
@file SoundEngine.cpp
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/


#include "EngineCore/stdafx.h"
#include "SoundEngine.h"


#include "swCommonLib/Common/MemoryLeaks.h"

namespace sw
{

// ================================ //
//
SoundEngine::SoundEngine( Engine* engine )
	: engine( engine )
{}

// ================================ //
//
SoundEngine::~SoundEngine()
{}

}	// sw

