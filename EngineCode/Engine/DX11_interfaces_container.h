#pragma once

/**@file DX11_interfaces_container.h
@brief Zaiwiera deklaracjê klasy DX11_interfaces_container i DX11_constant_buffers_container s³u¿¹cych do
Inicjowania i przechowywania obiektów DirectXa.*/

#include "WinDef.h"
#include "stdafx.h"

struct ID3D11Device;
struct ID3D11DeviceContext;
struct IDXGISwapChain;
struct ID3D11RenderTargetView;

typedef ID3D11DeviceContext DeviceContext;

#define GRAPHIC_ENGINE_INIT_OK		S_OK


/**@brief Klasa przechowuje w zmiennych statycznych najwa¿niejsze interfejsy
DirectX11.

Obiekty, które ich potrzebuj¹, powinny odziedziczyæ po tej klasie,
dziêki czemu maj¹ bezpoœredni dostêp.

Poza tym zawiera troszkê funkcji u³atwiaj¹cych tworzenie projektu DirectX.

Za zainicjowanie zmiennych odpowiednimi wartoœciami odpowiada tylko i wy³¹cznie
klasa Engine. Formalna inicjacja zmiennych statycznych odbywa siê w pliku 
DX11_interfaces_container.cpp (formalna, czyli mam na myœli, ¿e inicjacja nullptrami).

Klasa zawiera te¿ metody do zainicjowania odpowiednich komponentów.
Oczywiœcie wywo³aæ je ma prawo te¿ tylko i wy³¹cznie klasa Engine.

W przypadku b³êdów innych ni¿ zwróconych przez DirectXa funkcje zwracaj¹ makro
DXGI_ERROR_CANNOT_PROTECT_CONTENT. Mam nadziejê, ¿e to jest tak rzadko zwracana wartoœæ,
¿e bêdzie doœæ jednoznacznie mówiæ, ¿e b³¹d jest innego typu ni¿ DirectXowy. Niestety nie wiem wed³ug
jakiego klucza s¹ tworzone te kody b³êdów, wiêc nie moge stworzyæ swojego.*/

class DX11_interfaces_container
{
protected:
	static ID3D11Device*			device;					///<Zmienna s³u¿y do tworzenia obiektó, buforów tekstur i zasobów
	static ID3D11DeviceContext*		device_context;			///<Zmienna u¿ywana do renderowania sceny, ustawiania buforów, shaderów itp.
	static IDXGISwapChain*			swap_chain;				///<S³u¿y do prezentowania ramki na ekranie
	static ID3D11RenderTargetView*	render_target;			///<Widok bufora docelowego renderowania, czyli bufora tylnego
	static ID3D11DepthStencilView*	z_buffer_view;			///<Widok z-buffora
	static ID3D11InputLayout*		mesh_vertex_format;		///<Layout formatu wierzcho³ka u¿ywanego dla meshy
	static ID3D11InputLayout*		ui_vertex_format;		///<Layout formatu wierzcho³ka u¿ywanego w GUI
	static D3D_FEATURE_LEVEL		feature_level;			///<Opisuje zdolnoœci naszego sprzêtu
	static ID3DBlob*				compiled_vertex_shader;	///<Bufor zawieraj¹cy skompilowany domyœlny vertex shader
	static ID3DBlob*				compiled_pixel_shader;	///<Bufor zawieraj¹cy skompilowany domyœlny pixel shader
	static ID3D11VertexShader*		default_vertex_shader;	///<Obiekt domyœlnego vertex shadera
	static ID3D11PixelShader*		default_pixel_shader;	///<Obiekt domyœlnego piksel shadera
	static ID3D11SamplerState*		default_sampler;		///<Obiekt domyœlnego samplera

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
	inline void end_scene_and_present( ) { swap_chain->Present( 1, 0 ); }	///<Wywo³uje funkcjê swap_chain->Present w celu wyœwietlenia narysowanej sceny na monitorze
};



/**@brief Klasa zawiera wskaŸniki na bufory sta³ych zawieraj¹ce podstawowe zestawy
elementów przekazywanych do shaderów. S¹ to miedzy innymi macierze transformacji.

@note Je¿eli w jakimkolwiek miejscu s¹ u¿ywane obiekty z tej klasy (DX11_constant_buffers_container), a nie tylko z klasy bazowej,
to przy zwalnianiu nale¿y koniecznie wywo³aæ funkcjê release_DirectX tego obiektu, a nie klasy bazowej. Czyli innymi s³owy klasa
odpowiedzialna za zwalnianie musi odziedziczyæ po tym obiekcie nawet, je¿eli nie u¿ywa jego zmiennych.
W przeciwnym razie zostanie wywo³any funkcja wirtualna dla klasy bazowej.*/
class DX11_constant_buffers_container : public DX11_interfaces_container
{
protected:
	static ID3D11Buffer*				const_per_frame;	///<Bufor sta³ych zmieniaj¹cych siê nie czêœciej ni¿ co ramkê
	static ID3D11Buffer*				const_per_mesh;		///<Bufor sta³ych zmieniaj¹cy siê dla ka¿dej czêœci mesha
	static ID3D11DepthStencilState*		depth_enabled;		///<Do w³¹czania z-bufora
	static ID3D11DepthStencilState*		depth_disabled;		///<D wy³¹czania z-bufora

	void init_buffers(unsigned int size_per_frame, unsigned int size_per_mesh);
	void init_depth_states();

	void release_DirectX() override;
};

