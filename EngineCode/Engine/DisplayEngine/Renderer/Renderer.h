#pragma once
/**@file Renderer.h

Ten plik wybiera renderer, który bêdzie u¿ywany do renderowania.
Ka¿dy renderer definiuje siebie jako typ Renderer. W zale¿noœci od prze³¹cznika ENABLE_RENDERER
za³¹czany jest plik z odpowiednim rendererem.

@attention Nigdy nie nale¿y includowaæ renderera bezpoœrednio. Zawsze za poœrednictwem tego pliku.*/

#include "macros_switches.h"

enum RENDERER_USAGE
{
	USE_AS_DEFERRED,
	USE_AS_IMMEDIATE
};


#if ENABLE_RENDERER == DX11
	#include "DisplayEngine\Renderer\DX11Renderer.h"
#else
	#error No renderer set. Define macro ENABLE_RENDERER in file macros_switches.h to one of right values.
#endif