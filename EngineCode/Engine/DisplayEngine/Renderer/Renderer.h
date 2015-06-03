#pragma once
/**@file Renderer.h

Ten plik wybiera renderer, kt�ry b�dzie u�ywany do renderowania.
Ka�dy renderer definiuje siebie jako typ Renderer. W zale�no�ci od prze��cznika ENABLE_RENDERER
za��czany jest plik z odpowiednim rendererem.

@attention Nigdy nie nale�y includowa� renderera bezpo�rednio. Zawsze za po�rednictwem tego pliku.*/

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