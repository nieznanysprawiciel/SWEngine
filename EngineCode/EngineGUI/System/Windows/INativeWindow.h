#pragma once

#include "GraphicAPI/MeshResources.h"


namespace GUI
{


/**@brief Klasa bazowa kontenera dla natywnego okna danego systemu.

Natywne okno powinno udostêpniaæ przynajmniej swój render target i swapchain.
Ponadto mo¿e zawieraæ funkcje do obs³ugi prawdziwego okna, ale nie jest to konieczne.
GUI nie musi samo obs³ugiwaæ prawdziwych okien, mo¿e mieæ je dostarczone przez aplikacjê.*/
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
