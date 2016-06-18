#pragma once

#include "GraphicAPI/MeshResources.h"


namespace GUI
{


/**@brief Klasa bazowa kontenera dla natywnego okna danego systemu.

Natywne okno powinno udost�pnia� przynajmniej sw�j render target i swapchain.
Ponadto mo�e zawiera� funkcje do obs�ugi prawdziwego okna, ale nie jest to konieczne.
GUI nie musi samo obs�ugiwa� prawdziwych okien, mo�e mie� je dostarczone przez aplikacj�.*/
class INativeWindow
{
private:
protected:
public:
	INativeWindow() = default;
	~INativeWindow() = default;


	virtual		RenderTargetObject*			GetRenderTarget		();
	//virtual		SwapChainObject*			GetSwapChain		();
};



}	// GUI
