#pragma once

#include "WinDef.h"
#include "..\stdafx.h"

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
DX11_interfaces_container.cpp (formalna, czyli mam na my�li, �e inicjacja nullptrami).

Klasa zawiera te� metody do zainicjowania odpowiednich komponent�w.
Oczywi�cie wywo�a� je ma prawo te� tylko i wy��cznie klasa Engine.

W przypadku b��d�w innych ni� zwr�conych przez DirectXa funkcje zwracaj� makro
DXGI_ERROR_CANNOT_PROTECT_CONTENT. Mam nadziej�, �e to jest tak rzadko zwracana warto��,
�e b�dzie do�� jednoznacznie m�wi�, �e b��d jest innego typu ni� DirectXowy. Niestety nie wiem wed�ug
jakiego klucza s� tworzone te kody b��d�w, wi�c nie moge stworzy� swojego.*/

class DX11_interfaces_container
{
protected:
	static ID3D11Device*			device;
	static ID3D11DeviceContext*		device_context;
	static IDXGISwapChain*			swap_chain;
	static ID3D11RenderTargetView*	render_target;
	static ID3D11DepthStencilView*	z_buffer_view;
	static ID3D11InputLayout*		mesh_vertex_format;
	static ID3D11InputLayout*		ui_vertex_format;
	static D3D_FEATURE_LEVEL		feature_level;
	static ID3DBlob*				compiled_vertex_shader;
	static ID3DBlob*				compiled_pixel_shader;
	static ID3D11VertexShader*		default_vertex_shader;
	static ID3D11PixelShader*		default_pixel_shader;

	DX11_interfaces_container() = default;
	~DX11_interfaces_container() = default;

	int init_devices( int width, int height, HWND window, bool fullscreen, bool single_thread = true );
	int init_zBuffer( int width, int height );
	int init_vertex_shader( const std::wstring& file_name, const std::string& shader_name );
	int init_pixel_shader( const std::wstring& file_name, const std::string& shader_name);
	int init_mesh_vertex_format( D3D11_INPUT_ELEMENT_DESC* layout_desc, unsigned int array_size, ID3DBlob* shader );
	int init_ui_vertex_format( D3D11_INPUT_ELEMENT_DESC* layout_desc, unsigned int array_size, ID3DBlob* shader );

	void release_DirectX();

	void begin_scene( );
	inline void end_scene_and_present( ) { swap_chain->Present( 0, 0 ); }
};



/*Klasa zawiera wska�niki na bufory sta�ych zawieraj�ce podstawowe zestawy
element�w przekazywanych do shader�w. S� to miedzy innymi macierze transformacji.*/
class DX11_constant_buffers_container : public DX11_interfaces_container
{
protected:
	static ID3D11Buffer*		const_per_frame;
	static ID3D11Buffer*		const_per_mesh;

	void init_buffers(unsigned int size_per_frame, unsigned int size_per_mesh);
};

