#pragma once
/**@file Renderer.h

Ten plik wybiera renderer, który bêdzie u¿ywany do renderowania.
Ka¿dy renderer definiuje siebie jako typ Renderer. W zale¿noœci od prze³¹cznika ENABLE_RENDERER
za³¹czany jest plik z odpowiednim rendererem.

@attention Nigdy nie nale¿y includowaæ renderera bezpoœrednio. Zawsze za poœrednictwem tego pliku.*/

#include "Common\macros_switches.h"


#define RENDERER_H_INCLUDE

#include "IRenderer.h"


#if ENABLE_RENDERER == DX11
	#include "DisplayEngine\Renderer\DX11Renderer.h"
#else
	#error Renderer nie zosta³ ustawiony. Zdefiniuj makro ENABLE_RENDERER w pliku macros_switches.h.
#endif

#undef RENDERER_H_INCLUDE
