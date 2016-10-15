#pragma once
// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//


/// @note You can disable all headers in precompiled header with one macro.
/// All cpp files should be precompiled header independet and explicitly include needed headers.
#ifndef DISABLE_PRECOMPILED_HEADER


#include <cassert>

#include <string>
#include <vector>
#include <map>

#include <DirectXMath.h>

#include "Common/MacrosSwitches.h"
#include "Common/RTTR.h"

#include "MeshResources.h"
#include "BlendingState.h"
#include "DepthStencilState.h"
#include "RasterizerState.h"
#include "GraphicAPIConstants.h"
#include "ResourceManager.h"
#include "ResourcesFactory.h"
#include "SwapChain.h"

#undef min
#undef max
#undef RegisterClass


#endif // !DISABLE_PRECOMPILED_HEADER


