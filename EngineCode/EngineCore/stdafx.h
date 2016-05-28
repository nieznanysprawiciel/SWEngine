// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

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

#include "Common/MacrosSwitches.h"
#include "EngineCore/EventsManager/Event.h"

//#ifdef SHOW_MEMORY_LEAKS
//	#define _CRTDBG_MAP_ALLOC
//	#include <cstdlib>
//	#include <crtdbg.h>
//#endif
