#pragma once
/**
@file IncludeWindows.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/


// ================================ //
// This header includes windows.h and undefs all shitty macros

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#undef min
#undef max
#undef CreateWindow
#undef XBUTTON1
#undef XBUTTON2
