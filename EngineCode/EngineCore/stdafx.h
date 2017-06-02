#pragma once
// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//



#include "targetver.h"


/// @note You can disable all headers in precompiled header with one macro.
/// All cpp files should be precompiled header independet and explicitly include needed headers.
#ifndef DISABLE_PRECOMPILED_HEADER

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
//#include <windows.h>

// C RunTime Header Files
//#include <stdlib.h>
//#include <malloc.h>
//#include <memory.h>
//#include <tchar.h>


#define D3D_DEBUG_INFO
// TODO: reference additional headers your program requires here

#include <cassert>

#include <string>
#include <codecvt>
#include <vector>
#include <list>
#include <queue>
#include <unordered_map>
#include <map>

#include <DirectXMath.h>

#include "MainEngine/MacrosSwitches.h"
#include "EngineCore/EventsManager/Event.h"
#include "swCommonLib/Common/RTTR.h"

#include "EngineCore/Actors/ActorsManager.h"
#include "EngineCore/ControllersEngine/ControllersEngine.h"
#include "EngineCore/ControllersEngine/BaseClasses/BaseInputController.h"
#include "EngineCore/ControllersEngine/BaseClasses/BaseAIController.h"
#include "EngineCore/DisplayEngine/DisplayEngine.h"
#include "EngineCore/EventsManager/EventManager.h"
#include "EngineCore/GamePlay/IGamePlay.h"
#include "EngineCore/MainEngine/EngineContext.h"
#include "EngineCore/ModelsManager/AssetsManager.h"
#include "EngineCore/SoundEngine/SoundEngine.h"
#include "EngineCore/UIEngine/UI_Engine.h"

#undef min
#undef max
#undef RegisterClass

//#ifdef SHOW_MEMORY_LEAKS
//	#define _CRTDBG_MAP_ALLOC
//	#include <cstdlib>
//	#include <crtdbg.h>
//#endif


#endif // !DISABLE_PRECOMPILED_HEADER


