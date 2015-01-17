#pragma once

#include "WinDef.h"

struct ID3D11Device;
struct ID3D11DeviceContext;
struct IDXGISwapChain;
struct ID3D11RenderTargetView;

#define GRAPHIC_ENGINE_INIT_OK		S_OK


/*Klasa przechowuje w zmiennych statycznych najwa�niejsze interfejsy
DirectX11. Obiekty, kt�re ich potrzebuj�, powinny odziedziczy� po tej klasie,
dzi�ki czemu maj� bezpo�redni dost�p.

Poza tym zawiera troszk� funkcji u�atwiaj�cych tworzenie projektu DirectX.

Za zainicjowanie zmiennych odpowiednimi warto�ciami odpowiada tylko i wy��cznie
klasa Engine. Formalna inicjacja zmiennych statycznych odbywa si� w pliku 
Engine.cpp (formalna, czyli mam na my�li, �e inicjacja nullptrami).

Klasa zawiera te� metody do zainicjowania odpowiednich komponent�w.
Oczywi�cie wywo�a� je ma prawo te� tylko i wy��cznie klasa Engine.*/

class DX11_interfaces_container
{
protected:
	static ID3D11Device*			device;
	static ID3D11DeviceContext*		device_context;
	static IDXGISwapChain*			swap_chain;
	static ID3D11RenderTargetView*	render_target;
	static ID3D11DepthStencilView*	z_buffer_view;
	static D3D_FEATURE_LEVEL		feature_level;

	DX11_interfaces_container() = default;
	~DX11_interfaces_container() = default;

	int init_devices( int width, int height, HWND window, bool fullscreen, bool single_thread = true );
	int init_zBuffer( int width, int height );

	void release_DirectX();

	void begin_scene();
	inline void end_scene_and_present() { swap_chain->Present( 0, 0 ); }
};