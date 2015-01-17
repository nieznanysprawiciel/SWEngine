#pragma once

#include "WinDef.h"

struct ID3D11Device;
struct ID3D11DeviceContext;
struct IDXGISwapChain;
struct ID3D11RenderTargetView;

#define GRAPHIC_ENGINE_INIT_OK		S_OK


/*Klasa przechowuje w zmiennych statycznych najwa¿niejsze interfejsy
DirectX11. Obiekty, które ich potrzebuj¹, powinny odziedziczyæ po tej klasie,
dziêki czemu maj¹ bezpoœredni dostêp.

Poza tym zawiera troszkê funkcji u³atwiaj¹cych tworzenie projektu DirectX.

Za zainicjowanie zmiennych odpowiednimi wartoœciami odpowiada tylko i wy³¹cznie
klasa Engine. Formalna inicjacja zmiennych statycznych odbywa siê w pliku 
Engine.cpp (formalna, czyli mam na myœli, ¿e inicjacja nullptrami).

Klasa zawiera te¿ metody do zainicjowania odpowiednich komponentów.
Oczywiœcie wywo³aæ je ma prawo te¿ tylko i wy³¹cznie klasa Engine.*/

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