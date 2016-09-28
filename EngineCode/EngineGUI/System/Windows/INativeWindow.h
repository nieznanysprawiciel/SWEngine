#pragma once

#include "GraphicAPI/MeshResources.h"
#include "GraphicAPI/SwapChain.h"

#include <string>

namespace GUI
{



struct NativeWindowDescriptor
{
	WindowHandler			ParentHandle;
	unsigned short			Width;
	unsigned short			Height;
	unsigned short			PositionX;
	unsigned short			PositionY;
	bool					Fullscreen			: 1;
	bool					ShowWindow			: 1;	///< Show window immediately after creation.
	bool					AddExitButton		: 1;
	bool					AddMinimizeButton	: 1;
	bool					AddMaximizeButton	: 1;
	bool					AddFrame			: 1;
	bool					AddTitleBar			: 1;
	bool					AdjustSize			: 1;	///< Adjust window size to client area.
	std::string				WindowTitle;


// ================================ //
//
	NativeWindowDescriptor()
	{
		InitDefaults();
	}

	NativeWindowDescriptor( const std::string& windowTitle )
		:	WindowTitle( windowTitle )
	{
		InitDefaults();
	}

	void InitDefaults()
	{
		ParentHandle = nullptr;
		Width = 1024;
		Height = 768;
		PositionX = 0;
		PositionY = 0;
		Fullscreen = false;
		ShowWindow = true;
		AddExitButton = true;
		AddMinimizeButton = true;
		AddMaximizeButton = true;
		AddFrame = true;
		AddTitleBar = true;
		AdjustSize = true;
	}
};


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


	virtual		ResourcePtr< RenderTargetObject >	GetRenderTarget		() = 0;
	virtual		ResourcePtr< SwapChain >			GetSwapChain		() = 0;
};



}	// GUI
