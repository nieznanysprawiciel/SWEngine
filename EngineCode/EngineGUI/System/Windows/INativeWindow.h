#pragma once

#include "GraphicAPI/MeshResources.h"
#include "GraphicAPI/SwapChain.h"

#include <string>

namespace GUI
{

typedef void*	WindowHandler;

struct NativeWindowDescriptor
{
	WindowHandler			WindowHandle;
	WindowHandler			ParentHandle;
	unsigned short			Width;
	unsigned short			Height;
	unsigned short			PositionX;
	unsigned short			PositionY;
	bool					Fullscreen			: 1;
	bool					AddExitButton		: 1;
	bool					AddMinimizeButton	: 1;
	bool					AddFrame			: 1;
	bool					AddTitleBar			: 1;
	std::string				WindowTitle;
};


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
	virtual		SwapChain*					GetSwapChain		();
};



}	// GUI
