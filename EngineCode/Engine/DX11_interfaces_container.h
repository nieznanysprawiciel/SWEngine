#pragma once

#include "WinDef.h"
#include "..\stdafx.h"

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



/*Klasa zawiera wskaŸniki na bufory sta³ych zawieraj¹ce podstawowe zestawy
elementów przekazywanych do shaderów. S¹ to miedzy innymi macierze transformacji.*/
class DX11_constant_buffers_container : public DX11_interfaces_container
{
protected:
	static ID3D11Buffer*		const_per_frame;
	static ID3D11Buffer*		const_per_mesh;

	void init_buffers(unsigned int size_per_frame, unsigned int size_per_mesh);
};

