#include "stdafx.h"
#include "DX11_interfaces_container.h"
#include "DXGI.h"


#include "memory_leaks.h"


/*Inicjalizacja zmiennych statycznych.*/
ID3D11Device* DX11_interfaces_container::device = nullptr;
ID3D11DeviceContext* DX11_interfaces_container::device_context = nullptr;
IDXGISwapChain* DX11_interfaces_container::swap_chain = nullptr;
ID3D11RenderTargetView* DX11_interfaces_container::render_target = nullptr;
ID3D11DepthStencilView*	DX11_interfaces_container::z_buffer_view = nullptr;
ID3D11InputLayout* DX11_interfaces_container::mesh_vertex_format = nullptr;
ID3D11InputLayout* DX11_interfaces_container::ui_vertex_format = nullptr;
D3D_FEATURE_LEVEL DX11_interfaces_container::feature_level = D3D_FEATURE_LEVEL_11_0;
ID3DBlob* DX11_interfaces_container::compiled_vertex_shader = nullptr;
ID3DBlob* DX11_interfaces_container::compiled_pixel_shader = nullptr;
ID3D11VertexShader* DX11_interfaces_container::default_vertex_shader = nullptr;
ID3D11PixelShader* DX11_interfaces_container::default_pixel_shader = nullptr;
ID3D11SamplerState*	DX11_interfaces_container::default_sampler = nullptr;

ID3D11Buffer* DX11_constant_buffers_container::const_per_frame = nullptr;
ID3D11Buffer* DX11_constant_buffers_container::const_per_mesh = nullptr;
ID3D11DepthStencilState* DX11_constant_buffers_container::depth_enabled = nullptr;		///<Do w³¹czania z-bufora
ID3D11DepthStencilState* DX11_constant_buffers_container::depth_disabled = nullptr;		///<D wy³¹czania z-bufora



//----------------------------------------------------------------------------------------------//
//								Zwalnianie obiektów DirectXa									//
//----------------------------------------------------------------------------------------------//

/**@brief Zwalnia wszystkie stworzone obiekty DirectXa.
*/
void DX11_interfaces_container::release_DirectX( )
{
	// Bardzo wa¿ne jest ustawienie zmiennych na nullptr w razie, gdyby jakaœ inna klasa wywo³ywa³a destruktor

	//Zmienne pomocnicze
	if ( mesh_vertex_format )
		mesh_vertex_format->Release( ), mesh_vertex_format = nullptr;
	if ( ui_vertex_format )
		ui_vertex_format->Release( ), ui_vertex_format = nullptr;
	if ( compiled_vertex_shader )
		compiled_vertex_shader->Release( ), compiled_vertex_shader = nullptr;
	if ( compiled_pixel_shader )
		compiled_pixel_shader->Release( ), compiled_pixel_shader = nullptr;
	if ( default_vertex_shader )
		default_vertex_shader->Release( ), default_vertex_shader = nullptr;
	if ( default_pixel_shader )
		default_pixel_shader->Release( ), default_pixel_shader = nullptr;
	if ( default_sampler )
		default_sampler->Release( ), default_sampler = nullptr;

	if ( swap_chain )
		//DirectX nie potrafi siê zamkn¹æ w trybie pe³noekranowym, wiêc musimy go zmieniæ
		swap_chain->SetFullscreenState( FALSE, NULL );

	//Zmienne s³u¿¹ce do wyœwietlania
	if ( z_buffer_view )
		z_buffer_view->Release( ), z_buffer_view = nullptr;
	if ( swap_chain )
		swap_chain->Release( ), swap_chain = nullptr;
	if ( render_target )
		render_target->Release( ), render_target = nullptr;
	if ( device )
		device->Release( ), device = nullptr;
	if ( device_context )
		device_context->Release( ), device_context = nullptr;
}

//----------------------------------------------------------------------------------------------//
//								Tworzenie obiektów DirectXa										//
//----------------------------------------------------------------------------------------------//

/**@brief Tworzy obiekty swap_chain, device i device_context.

@param[in] width Szerokoœæ ekranu w pikselach
@param[in] height Wysokoœæ ekranu w pikselach
@param[in] window Uchwyt okna, w którym renderujemy
@param[in] fullscreen Tryb pe³noekranowy lub w oknie.
@param[in] single_thread Ustawiæ na true, je¿eli mamy tylko jeden w¹tek, który tworzy wszystkie obiekty
DirectXa typu bufory i shadery. Domyœlnie false.
@return Zwraca jedn¹ ze sta³ych, jak¹ zwraca DirectX.*/
int DX11_interfaces_container::init_devices( int width, int height, HWND window, bool fullscreen, bool single_thread )
{
	HRESULT result = S_OK;
	UINT createDeviceFlags = 0;

#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	if ( single_thread )
		// Domyœlnie obiekt ID3D11Device jest synchronizowany, ale mo¿na to wy³¹czyæ
		createDeviceFlags |= D3D11_CREATE_DEVICE_SINGLETHREADED;

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	UINT numFeatureLevels = ARRAYSIZE( featureLevels );


	DXGI_SWAP_CHAIN_DESC swap_chain_desc;
	ZeroMemory( &swap_chain_desc, sizeof(swap_chain_desc) );
	swap_chain_desc.BufferCount = 1;
	swap_chain_desc.BufferDesc.Width = width;
	swap_chain_desc.BufferDesc.Height = height;
	swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swap_chain_desc.BufferDesc.RefreshRate.Numerator = 60;
	swap_chain_desc.BufferDesc.RefreshRate.Denominator = 1;
	swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swap_chain_desc.OutputWindow = window;
	swap_chain_desc.SampleDesc.Count = 1;
	swap_chain_desc.SampleDesc.Quality = 0;
	swap_chain_desc.Windowed = TRUE;
	swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_DISCARD;
	swap_chain_desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	result = D3D11CreateDeviceAndSwapChain( nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
											createDeviceFlags, featureLevels, numFeatureLevels,
											D3D11_SDK_VERSION, &swap_chain_desc, &swap_chain,
											&device, &feature_level, &device_context );
	if ( FAILED(result) )
		return result;

	if ( fullscreen )
		swap_chain->SetFullscreenState( TRUE, nullptr );


	// Tworzymy RenderTargetView. W tym celu pobieramy wskaŸnik na obiekt tylniego bufora
	// i tworzymy z niego widok.
	ID3D11Texture2D* back_buffer = nullptr;
	result = swap_chain->GetBuffer( 0, __uuidof(ID3D11Texture2D), (LPVOID*)&back_buffer );
	if ( FAILED(result ) )
	{
		release_DirectX( );
		return result;
	}

	result = device->CreateRenderTargetView( back_buffer, nullptr, &render_target );
	if ( FAILED(result) )
	{
		release_DirectX( );
		return result;
	}
	//Zwracam uwagê, ¿e tutaj nie zwalniamy bufora. Zwalniamy tylko odwo³anie do niego, a bufor
	//zostanie zwolniony dopiero jak wszystkie istniej¹ce obiekty przestan¹ go u¿ywaæ.
	back_buffer->Release( );

	// Deklarujemy przekszta³cenie wspó³rzêdnych ekranowych, na wspó³rzedne znormalizowane
	D3D11_VIEWPORT view_port;
	view_port.Width = (FLOAT)width;
	view_port.Height = (FLOAT)height;
	view_port.MinDepth = 0.0f;
	view_port.MaxDepth = 1.0f;
	view_port.TopLeftX = 0;
	view_port.TopLeftY = 0;
	device_context->RSSetViewports( 1, &view_port );	//tworzymy tylko jeden viewport, ale noramlnie mo¿na wiecej, tylko po co

	return GRAPHIC_ENGINE_INIT_OK;
}

/**@brief Funkcja tworzy z-bufffer. Nastêpnie widok z bufora i widok tylnego bufora
s¹ ustawione jako cel dla funkcji renderuj¹cych renderowania.

Funkcji nie wolno wywo³ywaæ przed zainicjowaniem directXa.
@param[in] width Szerokoœæ ekranu w pikselach
@param[in] height Wysokoœæ ekranu w pikselach
@return Zwraca jedn¹ ze sta³ych, jak¹ zwraca DirectX.
*/
int DX11_interfaces_container::init_zBuffer( int width, int height )
{
	if ( !device )
		return DXGI_ERROR_ACCESS_DENIED;

	HRESULT result = S_OK;
	ID3D11Texture2D* zBuffer;


	D3D11_TEXTURE2D_DESC zBuffer_desc;
	ZeroMemory( &zBuffer_desc, sizeof(zBuffer_desc) );
	zBuffer_desc.Width = width;
	zBuffer_desc.Height = height;
	zBuffer_desc.MipLevels = 1;
	zBuffer_desc.ArraySize = 1;
	zBuffer_desc.Format = DXGI_FORMAT_D32_FLOAT;
	zBuffer_desc.SampleDesc.Count = 1;
	zBuffer_desc.SampleDesc.Quality = 0;
	zBuffer_desc.Usage = D3D11_USAGE_DEFAULT;
	zBuffer_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	zBuffer_desc.CPUAccessFlags = 0;
	zBuffer_desc.MiscFlags = 0;

	result = device->CreateTexture2D( &zBuffer_desc, nullptr, &zBuffer );

	if ( FAILED(result) )
	{
		release_DirectX( );
		return result;
	}

	// Create the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC zBufferView_desc;
	ZeroMemory( &zBufferView_desc, sizeof(zBufferView_desc) );
	zBufferView_desc.Format = zBuffer_desc.Format;
	zBufferView_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	zBufferView_desc.Texture2D.MipSlice = 0;

	result = device->CreateDepthStencilView( zBuffer, &zBufferView_desc, &z_buffer_view );

	zBuffer->Release();

	if ( FAILED(result) )
	{
		release_DirectX( );
		return result;
	}

	device_context->OMSetRenderTargets( 1, &render_target, z_buffer_view );

	return GRAPHIC_ENGINE_INIT_OK;
}



//----------------------------------------------------------------------------------------------//
//								Tworzenie formatów wierzcho³ków									//
//----------------------------------------------------------------------------------------------//

/**@brief Tworzy layout dla formatu wierzcho³ka podanego w parametrze.
Wynik jest zapisywany w polu mesh_vertex_format.

Aby stworzyæ layouty trzeba koniecznie podaæ jakiœ skompilowany obiekt shadera.
Nie jest to wygodne, ale tak niestety dzia³a DirectX 11.
Oznacza to, ¿e funkcjê tê mo¿na wywo³aæ dopiero wtedy, kiedy taki obiekt zostanie stworzony.
@param[in] layout_desc Deskryptor formatu layoutu.
@param[in] array_size Iloœc rekordów w layout_desc.
@param[in] shader WskaŸnik na bufor ze skompilowanym shaderem.
@return Zwraca jedn¹ ze sta³ych, jak¹ zwraca DirectX.
*/
int DX11_interfaces_container::init_mesh_vertex_format( D3D11_INPUT_ELEMENT_DESC* layout_desc, unsigned int array_size, ID3DBlob* shader )
{
	if ( !shader )
		return DXGI_ERROR_CANNOT_PROTECT_CONTENT;
	if ( !device )
		return DXGI_ERROR_CANNOT_PROTECT_CONTENT;

	// Tworzymy layout
	HRESULT result = device->CreateInputLayout( layout_desc, array_size, shader->GetBufferPointer( ),
												shader->GetBufferSize( ), &mesh_vertex_format );
	if ( FAILED( result ) )
		return result;

	return GRAPHIC_ENGINE_INIT_OK;
}

/**@brief Tworzy layout dla formatu wierzcho³ka podanego w parametrze.
Wynik jest zapisywany w polu ui_vertex_format.

Aby stworzyæ layouty trzeba koniecznie podaæ jakiœ skompilowany obiekt shadera.
Nie jest to wygodne, ale tak niestety dzia³a DirectX 11.
Oznacza to, ¿e funkcjê tê mo¿na wywo³aæ dopiero wtedy, kiedy taki obiekt zostanie stworzony.

Niestety podany shader musi mieæ zgodne wejœcie z layoutem, który tworzymy.
@param[in] layout_desc Deskryptor formatu layoutu.
@param[in] array_size Iloœc rekordów w layout_desc.
@param[in] shader WskaŸnik na bufor ze skompilowanym shaderem.
@return Zwraca jedn¹ ze sta³ych, jak¹ zwraca DirectX.*/
int DX11_interfaces_container::init_ui_vertex_format( D3D11_INPUT_ELEMENT_DESC* layout_desc, unsigned int array_size, ID3DBlob* shader )
{
	if ( !shader )
		return DXGI_ERROR_CANNOT_PROTECT_CONTENT;
	if ( !device )
		return DXGI_ERROR_CANNOT_PROTECT_CONTENT;

	// Tworzymy layout
	HRESULT result = device->CreateInputLayout( layout_desc, array_size, shader->GetBufferPointer( ),
												shader->GetBufferSize( ), &ui_vertex_format );
	if ( FAILED( result ) )
		return result;

	return GRAPHIC_ENGINE_INIT_OK;
}


//----------------------------------------------------------------------------------------------//
//								Tworzenie domyœlnych shaderów									//
//----------------------------------------------------------------------------------------------//

/**@brief Funkcja tworzy shader na podstawie podanego pliku, kompiluje go, a potem obudowuje klas¹
bêd¹c¹ interfejsem shadera. Parametr shader_name oznacza nazwê funkcji, od której zaczyna
siê wykonanie kodu shadera.

Funkcja ustawia ten shader w kontekœcie urz¹dzenia jako aktywny.
W trybie debug komunikaty kompilacji shadera s¹ przekazywane do okna output.

@param[in] file_name Nazwa pliku zawieraj¹cego shader.
@param[in] shader_name Nazwa shadera.
@return Zwraca jedn¹ ze sta³ych, jak¹ zwraca DirectX.*/
int DX11_interfaces_container::init_vertex_shader(const std::wstring& file_name, const std::string& shader_name)
{
	if ( !device || !device_context )
		return DXGI_ERROR_CANNOT_PROTECT_CONTENT;

	HRESULT result;
	// Troszkê zamieszania, ale w trybie debug warto wiedzieæ co jest nie tak przy kompilacji shadera
#ifdef _DEBUG
	ID3D10Blob* error_blob = nullptr;
#endif

	D3DX11CompileFromFile( file_name.c_str(), 0, 0, shader_name.c_str(), "vs_4_0",
						   0, 0, 0, &compiled_vertex_shader,
#ifdef _DEBUG
						   &error_blob,
#else
						   0,
#endif
						   &result );

	if ( FAILED( result ) )
	{
#ifdef _DEBUG
		if ( error_blob )
		{
			OutputDebugStringA( (char*)error_blob->GetBufferPointer( ) );
			error_blob->Release( );
		}
#endif
		return result;
	}

	result = device->CreateVertexShader( compiled_vertex_shader->GetBufferPointer(),
										 compiled_vertex_shader->GetBufferSize( ),
										 nullptr, &default_vertex_shader );
	
	if ( FAILED( result ) )
	{
		compiled_vertex_shader->Release();
		compiled_vertex_shader = nullptr;
		return result;
	}

	device_context->VSSetShader( default_vertex_shader, nullptr, 0 );
	return GRAPHIC_ENGINE_INIT_OK;
}

/**@brief Funkcja tworzy shader na podstawie podanego pliku, kompiluje go, a potem obudowuje klas¹
bêd¹c¹ interfejsem shadera. Parametr shader_name oznacza nazwê funkcji, od której zaczyna
siê wykonanie kodu shadera.

Poza tworzeniem domyœlnego shadera, funkcja tworzy domyœlny sampler.
W trybie debug komunikaty kompilacji shadera s¹ przekazywane do okna output.

Funkcja ustawia ten shader i sampler w kontekœcie urz¹dzenia jako aktywny.
@param[in] file_name Nazwa pliku zawieraj¹cego shader.
@param[in] shader_name Nazwa shadera.
@return Zwraca jedn¹ ze sta³ych, jak¹ zwraca DirectX.*/
int DX11_interfaces_container::init_pixel_shader( const std::wstring& file_name, const std::string& shader_name )
{
	if ( !device || !device_context )
		return DXGI_ERROR_CANNOT_PROTECT_CONTENT;

	HRESULT result;
#ifdef _DEBUG
	ID3D10Blob* error_blob = nullptr;
#endif

	D3DX11CompileFromFile( file_name.c_str( ), 0, 0, shader_name.c_str( ), "ps_4_0",
						   0, 0, 0, &compiled_pixel_shader,
#ifdef _DEBUG
						   &error_blob,
#else
						   0,
#endif
						   &result );

	if ( FAILED( result ) )
	{
#ifdef _DEBUG
		if ( error_blob )
		{
			OutputDebugStringA( (char*)error_blob->GetBufferPointer( ) );
			error_blob->Release( );
		}
#endif
		return result;
	}

	result = device->CreatePixelShader( compiled_pixel_shader->GetBufferPointer( ),
										compiled_pixel_shader->GetBufferSize( ),
										nullptr, &default_pixel_shader );

	if ( FAILED( result ) )
	{
		compiled_pixel_shader->Release( );
		compiled_pixel_shader = nullptr;
		return result;
	}

	// Przy okazji tworzymy domyslny sampler
	result = init_sampler();
	if ( FAILED( result ) )
	{
		compiled_pixel_shader->Release( );
		compiled_pixel_shader = nullptr;
		default_pixel_shader->Release();
		default_pixel_shader = nullptr;
		return result;
	}

	device_context->PSSetShader( default_pixel_shader, nullptr, 0 );
	device_context->PSSetSamplers( 0, 1, &default_sampler );
	return GRAPHIC_ENGINE_INIT_OK;
}

/**@brief Tworzy domyœlny smapler.
@return Zwraca jedn¹ ze sta³ych, jak¹ zwraca DirectX.*/
int DX11_interfaces_container::init_sampler( )
{
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory( &sampDesc, sizeof(sampDesc) );
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	HRESULT result = device->CreateSamplerState( &sampDesc, &default_sampler );

	// Niech funkcje zewnêtrzne sie martwi¹ jak by³ b³¹d
	return result;
}

//----------------------------------------------------------------------------------------------//
//								Funkcje pomocne przy wyœwietlaniu								//
//----------------------------------------------------------------------------------------------//


/**@brief Czyœcimy tylny bufor oraz z-bufor i wywo³ujemy funcjê inicjuj¹c¹ renderowanie.*/
void DX11_interfaces_container::begin_scene( )
{
	//Bufor tylny
	float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };	// red, green, blue, alpha
	device_context->ClearRenderTargetView( render_target, ClearColor );

	//Z-bufor
	device_context->ClearDepthStencilView( z_buffer_view, D3D11_CLEAR_DEPTH, 1.0f, 0 );
}


//----------------------------------------------------------------------------------------------//
//								Funkcje dla klasy DX11_constant_buffers_container				//
//----------------------------------------------------------------------------------------------//

/**@brief Tworzy bufory sta³ych dla shadera.

Bufory musz¹ mieæ rozmiar bêd¹cy wielokrotnoœci¹ 16 bajtów ze wzglêdu na rozmiar rejestrów GPU.
Je¿eli podane wartoœci takie nie s¹, to s¹ rozszerzane do tej wielokrotnoœci, ale taka sytuacja mo¿e to oznaczaæ,
¿e jest jakiœ b³¹d w programie.

@param[in] size_per_frame Rozmiar bufora const_per_frame.
@param[in] size_per_mesh Rozmiar bufora const_per_mesh.*/
void DX11_constant_buffers_container::init_buffers( unsigned int size_per_frame, unsigned int size_per_mesh )
{
	HRESULT result;

	// Bufory sta³ych musz¹ mieæ rozmiar bêd¹cy wielokrotnoœci¹ 16
	// Dobrze ¿e DirectX wypluwa jakieœ debugowe informacje, bo nie wiem, jakbym na to wpad³
	if ( size_per_frame % 16 )
	{
		size_per_frame = size_per_frame >> 4;		// Dzielimy na 16 (dzielenie ca³kowite)
		size_per_frame = (size_per_frame + 1) << 4;	// Najbli¿sza wielokrotnoœæ 16
	}

	if ( size_per_mesh % 16 )
	{
		size_per_mesh = size_per_mesh >> 4;			// Dzielimy na 16 (dzielenie ca³kowite)
		size_per_mesh = (size_per_mesh + 1) << 4;	// Najbli¿sza wielokrotnoœæ 16
	}

	// Tworzymy bufor sta³ych w ka¿dej ramce
	D3D11_BUFFER_DESC buffer_desc;
	ZeroMemory( &buffer_desc, sizeof(buffer_desc) );
	buffer_desc.Usage = D3D11_USAGE_DEFAULT;
	buffer_desc.ByteWidth = size_per_frame;
	buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	buffer_desc.CPUAccessFlags = 0;

	//Tworzymy bufor sta³ych, który jest niezmienny podczas wyœwietlania ramki
	result = device->CreateBuffer( &buffer_desc, nullptr, &const_per_frame );

	//Drugi bufor ró¿ni siê tylko wielkoœci¹
	buffer_desc.ByteWidth = size_per_mesh;

	//Tworzymy bufor sta³ych, do którego bêdziemy wpisywaæ wartoœci sta³e dla ka¿dego mesha (albo czêœci mesha)
	result = device->CreateBuffer( &buffer_desc, nullptr, &const_per_mesh );
}


void  DX11_constant_buffers_container::init_depth_states( )
{
	D3D11_DEPTH_STENCIL_DESC dsDesc;

	// Depth test parameters
	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;

	// Stencil test parameters
	dsDesc.StencilEnable = false;
	dsDesc.StencilReadMask = 0xFF;
	dsDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing
	dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing
	dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create depth stencil state
	device->CreateDepthStencilState( &dsDesc, &depth_enabled );

	device_context->OMSetDepthStencilState( depth_enabled, 1 );

	dsDesc.DepthEnable = false;
	device->CreateDepthStencilState( &dsDesc, &depth_disabled );
}

/**@brief Zwalania obiekty DirectXa. Funkcja wywo³uje tê sam¹ funkcjê z obiektu potomnego, ¿eby
zwolniæ wszystkie obiekty, które istniej¹.*/
void DX11_constant_buffers_container::release_DirectX()
{
	if ( const_per_frame )
		const_per_frame->Release( ), const_per_frame = nullptr;
	if ( const_per_mesh )
		const_per_mesh->Release( ), const_per_mesh = nullptr;
	if ( depth_enabled )
		depth_enabled->Release();
	if ( depth_disabled )
		depth_disabled->Release();

	// Zwalniamy te¿ wszystkie obiekty, które zwalnia³a klasa bazowa
	DX11_interfaces_container::release_DirectX();
}
