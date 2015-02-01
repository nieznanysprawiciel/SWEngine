// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


#define D3D_DEBUG_INFO
// TODO: reference additional headers your program requires here
#include <d3d9.h>
#include <DirectXMath.h>
#include <dinput.h>
#include <D3DX9.h>
#include <D3DX11.h>
#include <d3d11.h>
#include <string>
#include <codecvt>
#include <vector>
#include <list>
#include <queue>



#include "macros_switches.h"

#ifdef SHOW_MEMORY_LEAKS
	#define _CRTDBG_MAP_ALLOC
	#include <cstdlib>
	#include <crtdbg.h>
#endif
