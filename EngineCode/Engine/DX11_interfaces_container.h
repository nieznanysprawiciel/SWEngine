#pragma once

/**@file DX11_interfaces_container.h
@brief Zaiwiera deklaracj� klasy DX11_interfaces_container i DX11_constant_buffers_container s�u��cych do
Inicjowania i przechowywania obiekt�w DirectXa.*/

#include "WinDef.h"
#include "stdafx.h"

struct ID3D11Device;
struct ID3D11DeviceContext;
struct IDXGISwapChain;
struct ID3D11RenderTargetView;

typedef ID3D11DeviceContext DeviceContext;

#define GRAPHIC_ENGINE_INIT_OK		S_OK


/**@brief Klasa przechowuje w zmiennych statycznych najwa�niejsze interfejsy
DirectX11.

Obiekty, kt�re ich potrzebuj�, powinny odziedziczy� po tej klasie,
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
	static ID3D11Device*			device;					///<Zmienna s�u�y do tworzenia obiekt�, bufor�w tekstur i zasob�w
	static ID3D11DeviceContext*		device_context;			///<Zmienna u�ywana do renderowania sceny, ustawiania bufor�w, shader�w itp.
	static IDXGISwapChain*			swap_chain;				///<S�u�y do prezentowania ramki na ekranie
	static ID3D11RenderTargetView*	render_target;			///<Widok bufora docelowego renderowania, czyli bufora tylnego
	static ID3D11DepthStencilView*	z_buffer_view;			///<Widok z-buffora
	static ID3D11InputLayout*		mesh_vertex_format;		///<Layout formatu wierzcho�ka u�ywanego dla meshy
	static ID3D11InputLayout*		ui_vertex_format;		///<Layout formatu wierzcho�ka u�ywanego w GUI
	static D3D_FEATURE_LEVEL		feature_level;			///<Opisuje zdolno�ci naszego sprz�tu
	static ID3DBlob*				compiled_vertex_shader;	///<Bufor zawieraj�cy skompilowany domy�lny vertex shader
	static ID3DBlob*				compiled_pixel_shader;	///<Bufor zawieraj�cy skompilowany domy�lny pixel shader
	static ID3D11VertexShader*		default_vertex_shader;	///<Obiekt domy�lnego vertex shadera
	static ID3D11PixelShader*		default_pixel_shader;	///<Obiekt domy�lnego piksel shadera
	static ID3D11SamplerState*		default_sampler;		///<Obiekt domy�lnego samplera

	DX11_interfaces_container() = default;
	~DX11_interfaces_container() = default;

	int init_devices( int width, int height, HWND window, bool fullscreen, bool single_thread = true );
	int init_zBuffer( int width, int height );
	int init_vertex_shader( const std::wstring& file_name, const std::string& shader_name );
	int init_pixel_shader( const std::wstring& file_name, const std::string& shader_name);
	int init_mesh_vertex_format( D3D11_INPUT_ELEMENT_DESC* layout_desc, unsigned int array_size, ID3DBlob* shader );
	int init_ui_vertex_format( D3D11_INPUT_ELEMENT_DESC* layout_desc, unsigned int array_size, ID3DBlob* shader );
	int init_sampler();


	virtual void release_DirectX();

	void begin_scene( );
	inline void end_scene_and_present( ) { swap_chain->Present( 1, 0 ); }	///<Wywo�uje funkcj� swap_chain->Present w celu wy�wietlenia narysowanej sceny na monitorze
};



/**@brief Klasa zawiera wska�niki na bufory sta�ych zawieraj�ce podstawowe zestawy
element�w przekazywanych do shader�w. S� to miedzy innymi macierze transformacji.

@note Je�eli w jakimkolwiek miejscu s� u�ywane obiekty z tej klasy (DX11_constant_buffers_container), a nie tylko z klasy bazowej,
to przy zwalnianiu nale�y koniecznie wywo�a� funkcj� release_DirectX tego obiektu, a nie klasy bazowej. Czyli innymi s�owy klasa
odpowiedzialna za zwalnianie musi odziedziczy� po tym obiekcie nawet, je�eli nie u�ywa jego zmiennych.
W przeciwnym razie zostanie wywo�any funkcja wirtualna dla klasy bazowej.*/
class DX11_constant_buffers_container : public DX11_interfaces_container
{
protected:
	static ID3D11Buffer*				const_per_frame;	///<Bufor sta�ych zmieniaj�cych si� nie cz�ciej ni� co ramk�
	static ID3D11Buffer*				const_per_mesh;		///<Bufor sta�ych zmieniaj�cy si� dla ka�dej cz�ci mesha
	static ID3D11DepthStencilState*		depth_enabled;		///<Do w��czania z-bufora
	static ID3D11DepthStencilState*		depth_disabled;		///<D wy��czania z-bufora

	void init_buffers(unsigned int size_per_frame, unsigned int size_per_mesh);
	void init_depth_states();

	void release_DirectX() override;
};

